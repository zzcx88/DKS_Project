#pragma once

class CGameObject;
namespace BT
{
	class CNode
	{
	public:
		virtual bool Invoke(CGameObject* pObj) = 0;
	};

	class CompositeNode : public CNode
	{
	public:
		void AddChild(CNode* node)
		{
			mChildren.emplace_back(node);
		}
		const list<CNode*>& GetChildren()
		{
			return mChildren;
		}
	private:
		list<CNode*> mChildren;
	};


	class Selector : public CompositeNode
	{
	public:
		virtual bool Invoke(CGameObject* pObj) override
		{
			for (auto node : GetChildren())
			{
				if (node->Invoke(pObj))
					return true;
			}
			return false;
		}
	};


	class Sequence : public CompositeNode
	{
	public:
		virtual bool Invoke(CGameObject* pObj) override
		{
			for (auto node : GetChildren())
			{
				if (!node->Invoke(pObj))
					return false;
			}
			return true;
		}
	};
}

class IsEnemyNear : public BT::CompositeNode
{
public:
	IsEnemyNear() {}
	virtual ~IsEnemyNear(){}

public:
	virtual bool Invoke(CGameObject* pObj) override
	{
		XMFLOAT3 xmf3Pos, xmf3PlayerPos, xmf3TargetVector;
		xmf3Pos = pObj->GetPosition();
		xmf3PlayerPos = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player", OBJ_PLAYER)->GetPosition();
		xmf3TargetVector = Vector3::Subtract(xmf3Pos, xmf3PlayerPos);
		float Lenth = sqrt(xmf3TargetVector.x * xmf3TargetVector.x + xmf3TargetVector.y * xmf3TargetVector.x + xmf3TargetVector.z * xmf3TargetVector.z);

		if (Lenth < 3000)
		{
			//cout << Lenth << endl;
			return true;
		}
		else
			return false;
	}
};

class MoveToEnemy : public BT::CompositeNode
{
public:
	MoveToEnemy() {}
	virtual ~MoveToEnemy() {}

public:
	virtual bool Invoke(CGameObject* pObj) override
	{
		if (pObj->m_bAllyCollide == true)
			return false;
		XMFLOAT3 xmf3Pos, xmf3PlayerPos, xmf3TargetVector;
		xmf3Pos = pObj->GetPosition();
		xmf3PlayerPos = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player", OBJ_PLAYER)->GetPosition();

		/*float Lenth = sqrt(xmf3TargetVector.x * xmf3TargetVector.x + xmf3TargetVector.y * xmf3TargetVector.x + xmf3TargetVector.z * xmf3TargetVector.z);
		cout << Lenth << endl;*/

		float theta = 50.f * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed();
		xmf3TargetVector = Vector3::Subtract(xmf3PlayerPos, xmf3Pos);
		xmf3TargetVector = Vector3::Normalize(xmf3TargetVector);
		XMFLOAT3 xmfAxis = Vector3::CrossProduct(pObj->m_xmf3Look, xmf3TargetVector);
		xmfAxis = Vector3::Normalize(xmfAxis);
		pObj->Move(DIR_FORWARD, 200 * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed() , false);
		pObj->RotateFallow(&xmfAxis, theta);
		return true;
	}
};

class MoveException : public BT::CompositeNode
{
public:
	MoveException() {}
	virtual ~MoveException() {}

public:
	virtual bool Invoke(CGameObject* pObj) override
	{
		//���� ���ٸ� ���� ��½�Ų��.
		if (pObj->GetPosition().y < 1450.f)
		{
			XMFLOAT3 xmf3Pos, xmf3TargetPos, xmf3PlayerPos, xmf3TargetVector;
			xmf3Pos = pObj->GetPosition();
			xmf3PlayerPos = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player", OBJ_PLAYER)->GetPosition();
			xmf3TargetPos = XMFLOAT3(xmf3PlayerPos.x,1800, xmf3PlayerPos.z);

			float theta = 50.f * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed();
			xmf3TargetVector = Vector3::Subtract(xmf3TargetPos, xmf3Pos);
			xmf3TargetVector = Vector3::Normalize(xmf3TargetVector);
			XMFLOAT3 xmfAxis = Vector3::CrossProduct(pObj->m_xmf3Look, xmf3TargetVector);
			xmfAxis = Vector3::Normalize(xmfAxis);
			pObj->Move(DIR_FORWARD, 200 * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed(), false);
			pObj->RotateFallow(&xmfAxis, theta);
		}
		//��ó�� �Ʊ��� �ִٸ� ������ �������� �����
		if (pObj->m_bAllyCollide == true)
		{
			pObj->Move(DIR_FORWARD, 230 * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed(), false);
			return true;
		}
		return false;
	}
};

//���� ���� ������ ������ ������ ���� ��ȣ�� ������.
class LockOn : public BT::CompositeNode
{
public:
	LockOn() {}
	virtual ~LockOn() {}

public:
	virtual bool Invoke(CGameObject* pObj) override
	{
		if (pObj->m_bAiLockOn == false)
		{
			//pObj->m_b_AiCanFire = false;
			XMFLOAT3 xmf3TargetVector = Vector3::Subtract(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player", OBJ_PLAYER)->GetPosition(), pObj->GetPosition());
			xmf3TargetVector = Vector3::Normalize(xmf3TargetVector);
			float xmfAxis = Vector3::DotProduct(pObj->GetLook(), xmf3TargetVector);
			if ((xmfAxis > 0.9f || xmfAxis < -0.9f) && xmfAxis > 0.f)
			{
				pObj->m_bAiLockOn = true;
				/*CMissle* pMissle;
				XMFLOAT3* temp;
				temp = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player", OBJ_PLAYER)->GetPositionForMissle();
				pMissle = new CMissle(pObj);
				pMissle->m_xmfTarget = temp;
				pMissle->m_bLockOn = true;
				pMissle->SetPosition(pObj->GetPosition());
				GET_MANAGER<ObjectManager>()->AddObject(L"enemy_missle", pMissle, OBJ_MISSLE);*/
				return true;
			}
		}
		else
		{
			pObj->m_bAiLockOn = false;
			return false;
		}
	}
};

//�̻���, ���� �� �߻��Ѵ�.
class Attack : public BT::CompositeNode
{
public:
	Attack() {}
	virtual ~Attack() {}

public:
	virtual bool Invoke(CGameObject* pObj) override
	{
		if (pObj->m_bAiLockOn == true && pObj->m_bAiCanFire == true)
		{
			pObj->m_bAiCanFire = false;
			/*XMFLOAT3 xmf3TargetVector = Vector3::Subtract(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player", OBJ_PLAYER)->GetPosition(), pObj->GetPosition());
			xmf3TargetVector = Vector3::Normalize(xmf3TargetVector);
			float xmfAxis = Vector3::DotProduct(pObj->GetLook(), xmf3TargetVector);
			if ((xmfAxis > 0.9f || xmfAxis < -0.9f) && xmfAxis > 0.f)*/
			{
				CMissle* pMissle;
				XMFLOAT3* temp;
				temp = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player", OBJ_PLAYER)->GetPositionForMissle();
				pMissle = new CMissle(pObj);
				pMissle->m_xmfTarget = temp;
				pMissle->m_bLockOn = true;
				pMissle->SetPosition(pObj->GetPosition());
				GET_MANAGER<ObjectManager>()->AddObject(L"enemy_missle", pMissle, OBJ_MISSLE);
				return true;
			}
		}
		else
			return false;
	}
};

//ȸ���Ѵ�(ȸ���Ѵ�)
class Evade : public BT::CompositeNode
{
public:
	Evade() {}
	virtual ~Evade() {}

public:
	virtual bool Invoke(CGameObject* pObj) override
	{
		
	}
};
//������ ��ġ�� �����δ�.

//�����Ѵ�(��, ���� ������Ʈ�� ����).
