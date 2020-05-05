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
	BT::Sequence* seqFindPlayer = new BT::Sequence(); // �÷��̾ ã�� ������
	BT::Sequence* seqMoveToPlayer = new BT::Sequence(); // �÷��̾�� �̵��ϴ� ������
	root->AddChild(selector);
	selector->AddChild(seqFindPlayer);
	selector->AddChild(seqMoveToPlayer);

	m_mapNode.insert(MAPNODE::value_type(AI_AIRCRAFT, root->GetChildren()));
}

void AIManager::DoAction(AITYPE Type)
{
}
