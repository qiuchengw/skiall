#include "ActorIKTarget.hpp"
#include "BlockReader.hpp"
#include <cmath>

using namespace nima;

ActorIKTarget::InfluencedBone::InfluencedBone() : boneIndex(0), bone(nullptr)
{

}

ActorIKTarget::ActorIKTarget() :
	ActorNode(ComponentType::ActorIKTarget),

	m_NumInfluencedBones(0),
	m_InfluencedBones(nullptr),
	m_InvertDirection(false),
	m_Strength(0.0f),
	m_Order(0),
	m_Bone1(nullptr),
	m_Bone1Child(nullptr),
	m_Bone2(nullptr),
	m_ChainLength(0),
	m_Chain(nullptr)
{

}

ActorIKTarget::~ActorIKTarget()
{
	delete m_InfluencedBones;
	delete m_Chain;
}

ActorNode* ActorIKTarget::makeInstance(Actor* resetActor)
{
	ActorIKTarget* instanceNode = new ActorIKTarget();
	instanceNode->copy(this, resetActor);
	return instanceNode;
}

void ActorIKTarget::copy(ActorIKTarget* node, Actor* resetActor)
{
	Base::copy(node, resetActor);

	m_Order = node->m_Order;
	m_InvertDirection = node->m_InvertDirection;
	m_Strength = node->m_Strength;
	m_NumInfluencedBones = node->m_NumInfluencedBones;
	m_InfluencedBones = new InfluencedBone[m_NumInfluencedBones];
	for (int i = 0; i < m_NumInfluencedBones; i++)
	{
		InfluencedBone& ib = m_InfluencedBones[i];
		ib.boneIndex = node->m_InfluencedBones[i].boneIndex;
	}
}

ActorIKTarget* ActorIKTarget::read(Actor* actor, BlockReader* reader, ActorIKTarget* node)
{
	if (node == nullptr)
	{
		node = new ActorIKTarget();
	}

	Base::read(actor, reader, node);

	node->m_Order = reader->readUnsignedShort();
	node->m_Strength = reader->readFloat();
	node->m_InvertDirection = reader->readByte() == 1;

	node->m_NumInfluencedBones = (int)reader->readByte();
	if (node->m_NumInfluencedBones > 0)
	{
		node->m_InfluencedBones = new InfluencedBone[node->m_NumInfluencedBones];
		for (int i = 0; i < node->m_NumInfluencedBones; i++)
		{
			InfluencedBone& ib = node->m_InfluencedBones[i];
			ib.boneIndex = reader->readUnsignedShort();
		}
	}

	return node;
}

void ActorIKTarget::resolveComponentIndices(ActorComponent** components)
{
	Base::resolveComponentIndices(components);
	if (m_InfluencedBones != nullptr)
	{
		for (int i = 0; i < m_NumInfluencedBones; i++)
		{
			InfluencedBone& ib = m_InfluencedBones[i];
			ib.bone = static_cast<ActorBone*>(components[ib.boneIndex]);
			ib.bone->addDependent(this);
		}

		m_Bone1 = m_InfluencedBones[0].bone;
		m_Bone2 = m_InfluencedBones[m_NumInfluencedBones - 1].bone;
		ActorBone* b1c = m_Bone2;
		ActorBone* b1 = m_Bone1;

		if(m_NumInfluencedBones > 1)
		{
			while (b1c != nullptr && b1c->parent() != b1)
			{
				ActorNode* n = b1c->parent();
				if (n != nullptr && n->type() == ComponentType::ActorBone)
				{
					b1c = dynamic_cast<ActorBone*>(n);
				}
				else
				{
					break;
				}
			}
		}
		m_Bone1Child = b1c;

		m_ChainLength = 0;
		ActorNode* end = m_Bone2;
		while (end != nullptr && end != b1->parent())
		{
			m_ChainLength++;

			ActorNode* n = end->parent();
			if (n != nullptr && n->type() == ComponentType::ActorBone)
			{
				end = n;
			}
			else
			{
				end = nullptr;
			}
		}

		m_Chain = new BoneChain[m_ChainLength];
		end = m_Bone2;
		int chainIndex = 0;
		while (end != nullptr && end != b1->parent())
		{
			BoneChain& bc = m_Chain[chainIndex++];
			bc.bone = reinterpret_cast<ActorBone*>(end);
			bc.angle = 0.0f;
			ActorNode* n = end->parent();
			if (n != nullptr && n->type() == ComponentType::ActorBone)
			{
				end = n;
			}
			else
			{
				end = nullptr;
			}

			bc.included = doesInfluence(bc.bone) || doesInfluence(reinterpret_cast<ActorBone*>(end)); // end is either null or an actorbone (for sure) here.
			
		}
	}
}

bool ActorIKTarget::doesInfluence(ActorBone* bone)
{
	if (bone == nullptr)
	{
		return false;
	}
	for (int i = 0; i < m_NumInfluencedBones; i++)
	{
		if (m_InfluencedBones[i].bone == bone)
		{
			return true;
		}
	}
	return false;
}

int ActorIKTarget::order()
{
	return m_Order;
}

bool ActorIKTarget::needsSolve()
{
	return isWorldDirty() || isDirty();
}

float ActorIKTarget::strength() const
{
	return m_Strength;
}

void ActorIKTarget::strength(float s)
{
	if (m_Strength != s)
	{
		m_Strength = s;
		markDirty();
	}
}

bool ActorIKTarget::suppressMarkDirty()
{
	return Base::suppressMarkDirty();
}

void ActorIKTarget::suppressMarkDirty(bool suppressIt)
{
	Base::suppressMarkDirty(suppressIt);
}

void ActorIKTarget::solveStart()
{
	if (m_Bone1 == nullptr)
	{
		return;
	}

	// Reset all rotation overrides to FK ones,
	if (m_Bone1Child != m_Bone2)
	{
		m_Bone1Child->setRotationOverride(m_Bone1Child->rotation());
	}

	for (int i = 0; i < m_NumInfluencedBones; i++)
	{
		InfluencedBone& ib = m_InfluencedBones[i];
		ib.bone->setRotationOverride(ib.bone->rotation());
	}
}


void ActorIKTarget::solve1(ActorBone* b1, Vec2D& worldTargetTranslation)
{
	Mat2D iworld;
	Mat2D::invert(iworld, b1->worldTransform());

	Vec2D targetLocal;
	Vec2D::transform(targetLocal, worldTargetTranslation, iworld);

	float a = atan2(targetLocal[1], targetLocal[0]);

	b1->setRotationOverride(b1->overrideRotationValue() + a);
}

void ActorIKTarget::solve2(ActorBone* b1, ActorBone* b2, Vec2D& worldTargetTranslation, bool invert)
{
	const Mat2D& world = b1->parent()->worldTransform();
	Mat2D iworld;
	ActorBone* b1c = b2;
	while (b1c != nullptr && b1c->parent() != b1)
	{
		ActorNode* n = b1c->parent();
		if (n != nullptr && n->type() == ComponentType::ActorBone)
		{
			b1c = reinterpret_cast<ActorBone*>(n);
		}
		else
		{
			b1c = nullptr;
		}
	}

	ActorNode* b1pn = b1->parent();
	// Get the world transform to the bone tip position.
	if (b1pn->type() == ComponentType::ActorBone)
	{
		Mat2D t;
		t[4] = reinterpret_cast<ActorBone*>(b1pn)->length();
		Mat2D::multiply(t, world, t);
		Mat2D::invert(iworld, t);
	}
	else
	{
		Mat2D::invert(iworld, world);
	}

	Vec2D pA; b1->worldTranslation(pA);
	Vec2D pC; b1->tipWorldTranslation(pC);
	Vec2D pB; b2->tipWorldTranslation(pB);
	Vec2D pBT(worldTargetTranslation);

	Vec2D::transform(pA, pA, iworld);
	Vec2D::transform(pC, pC, iworld);
	Vec2D::transform(pB, pB, iworld);
	Vec2D::transform(pBT, pBT, iworld);

	// http://mathworld.wolfram.com/LawofCosines.html
	Vec2D av; Vec2D::subtract(av, pB, pC);
	float a = Vec2D::length(av);

	Vec2D bv; Vec2D::subtract(bv, pC, pA);
	float b = Vec2D::length(bv);

	Vec2D cv; Vec2D::subtract(cv, pBT, pA);
	float c = Vec2D::length(cv);

	float A = acos(std::max(-1.0f, std::min(1.0f, (-a * a + b * b + c * c) / (2.0f * b * c))));
	float C = acos(std::max(-1.0f, std::min(1.0f, (a * a + b * b - c * c) / (2.0f * a * b))));

	float angleCorrection = 0.0f;
	if (b1c != b2)
	{
		Mat2D iworld2;
		Mat2D::invert(iworld2, b1c->worldTransform());

		Vec2D pa2; b2->tipWorldTranslation(pa2);
		Vec2D tipBone2Local; Vec2D::transform(pa2, pa2, iworld2);
		angleCorrection = -atan2(tipBone2Local[1], tipBone2Local[0]);
	}
	if (invert)
	{
		b1->setRotationOverride(atan2(pBT[1], pBT[0]) - A);
		b1c->setRotationOverride(-C + M_PI + angleCorrection);
	}
	else
	{
		b1->setRotationOverride(A + atan2(pBT[1], pBT[0]));
		b1c->setRotationOverride(C - M_PI + angleCorrection);
	}
}

void ActorIKTarget::solve()
{
	if (m_Chain == nullptr)
	{
		return;
	}

	Vec2D worldTargetTranslation;
	const Mat2D& wt = worldTransform();
	worldTargetTranslation[0] = wt[4];
	worldTargetTranslation[1] = wt[5];

	for (int i = 0; i < m_ChainLength; i++)
	{
		BoneChain& fk = m_Chain[i];
		fk.angle = fk.bone->overrideRotationValue();
	}

	if (m_NumInfluencedBones == 1)
	{
		solve1(m_InfluencedBones[0].bone, worldTargetTranslation);
	}
	else if (m_NumInfluencedBones == 2)
	{
		solve2(m_InfluencedBones[0].bone, m_InfluencedBones[1].bone, worldTargetTranslation, m_InvertDirection);
	}
	else
	{
		for (int i = 0; i < m_NumInfluencedBones - 1; i++)
		{
			solve2(m_InfluencedBones[i].bone, m_Bone2, worldTargetTranslation, m_InvertDirection);
		}
	}

	// At the end, mix the FK angle with the IK angle by strength
	if (m_Strength != 1.0f)
	{
		float im = 1.0f - m_Strength;
		for (int i = 0; i < m_ChainLength; i++)
		{
			BoneChain& fk = m_Chain[i];
			if (fk.included)
			{
				fk.bone->setRotationOverride(fk.bone->overrideRotationValue() * m_Strength + fk.angle * im);
			}
		}
	}
}