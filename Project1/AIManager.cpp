#include "stdafx.h"
#include "AIManager.h"


AIManager::AIManager()
{
}

AIManager::~AIManager()
{
}

void AIManager::LoadAction()
{
}

void AIManager::MakeAction()
{
	BT::Sequence* root = new BT::Sequence(); // ��Ʈ ���(������ ���� ����)
	BT::Selector* selector = new BT::Selector(); // ������

	BT::Sequence* seqPatrol = new BT::Sequence(); // ���� ������
	BT::Sequence* seqAvoidAction = new BT::Sequence(); 
	BT::Sequence* seqMove = new BT::Sequence();

	BT::Sequence* seqOffence = new BT::Sequence();

	BT::CNode* EnemyNear = new IsEnemyNear();

	
	root->AddChild(selector);
	selector->AddChild(seqPatrol);
	seqPatrol->AddChild(EnemyNear);

	m_mapNode.insert(MAPNODE::value_type(AI_AIRCRAFT, root->GetChildren()));
}

void AIManager::DoAction(AITYPE Type, CGameObject* pObj)
{
	if (!pObj->m_bAIEnable)
	{
		MakeAction();
		pObj->m_bAIEnable = true;
	}
	NODELIST::iterator iter;
	iter = m_mapNode.find(AI_AIRCRAFT)->second.begin();

	(*iter)->Invoke(pObj);
	/*while (!(*iter)->Invoke())
	{

	}*/
}
