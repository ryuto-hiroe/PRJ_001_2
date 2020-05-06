#pragma once


//=======================================================
//
// �R���|�[�l���g�̃x�[�X�N���X
//
//=======================================================
class BaseComponent : public std::enable_shared_from_this<BaseComponent>
{
public:

	virtual ~BaseComponent() {}

	//===============================
	//
	// �������֐�
	//
	//===============================
	// �I�u�W�F�N�g�������ɌĂ΂��
	virtual void Awake(){}

	// 
	virtual void Start(){}

	//===============================
	//
	// �X�V�֐�
	//
	//===============================
	//
	virtual void Update() {}

	//===============================
	//
	// �`��֐�
	//
	//===============================
	//
	virtual void Draw(){}

	//===============================
	//
	// Serialize / Deserialize
	//
	//===============================


	//===============================
	//
	// Editor�p
	//
	//===============================
	// ImGui�̏���
	virtual void ImGuiUpdate();

	//===============================
	//
	// ��{���擾
	//
	//===============================
	// �N���X���擾
	std::string ClassName() {
		return &typeid(*this).name()[6];
	}

	// 
	// ������擾
	std::weak_ptr<Object> GetOwner()const { return m_pOwner; }
	// ������ݒ�
	void SetOwner(std::shared_ptr<Object> owner) { m_pOwner = owner; }

protected:
	// �I�[�i�[�I�u�W�F�N�g�̃|�C���^
	std::weak_ptr<Object> m_pOwner;

	//
	bool m_isDelete = false;

	// �L�� / �����t���O
	bool m_enable = true;
};