#pragma once
#include "SingletonBase.h"
#include "CNode.h"

class CNode;
class AIManager : public SingletonBase<AIManager>
{
public:
	AIManager();
	virtual ~AIManager();

public:
	CGameObject* m_pActorObject;
public:
	typedef list<BT::CNode*> NODELIST;
	typedef std::unordered_multimap<AITYPE , NODELIST>	MAPNODE;

	const NODELIST& GetAIFromType(AITYPE AIType) { return m_mapNode.find(AIType)->second; }

	//�ൿ ������ �о�´�.
	void LoadAction();
	//�о�� ������ �ൿƮ���� �����.
	void MakeAction();
	//������� �ൿƮ���� �����Ѵ�
	void DoAction(AITYPE Type, CGameObject* pObj);

private:
	std::unordered_multimap<AITYPE, NODELIST>			m_mapNode;
};

