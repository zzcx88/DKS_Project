#pragma once
#include "SingletonBase.h"

class AIManager : public SingletonBase<AIManager>
{
public:
	AIManager();
	virtual ~AIManager();

public:
	CGameObject* m_pActorObject;
public:
	typedef std::unordered_multimap<const TCHAR*, CGameObject*>	MAPTREE;

	//�ൿ ������ �о�´�.
	void LoadAction();
	//�о�� ������ �ൿƮ���� �����.
	void MakeAction();
	//������� �ൿƮ���� �����Ѵ�
	void DoAction();
	
};

