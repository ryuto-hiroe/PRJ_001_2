#pragma once

//==================================================================
//
// �Z�p�N���X���܂Ƃ߂�����
//
//==================================================================

class RgVec2;
class RgVec3;
class RgVec4;
class RgMatrix;


//==================================================================
// 2D�x�N�g��
//	DirectX::SimpleMath::Vector2���p�����Ă��܂��B
//==================================================================
class RgVec2 : public DirectX::SimpleMath::Vector2
{
public:

	//====================================
	// �������E�Z�b�g
	//====================================

	// �R���X�g���N�^
	RgVec2() : DirectX::SimpleMath::Vector2() {}
	RgVec2(float fx, float fy) : DirectX::SimpleMath::Vector2(fx, fy){}
	RgVec2(const DirectX::XMFLOAT2& In) : DirectX::SimpleMath::Vector2(In) {}
	RgVec2(DirectX::FXMVECTOR V) : DirectX::SimpleMath::Vector2(V) {}

	// �Z�b�g
	void Set(float fx, float fy) {
		x = fx;
		y = fy;
	}

	//================================================
	//
	// �擾
	//
	//================================================
	// �擪�A�h���X�� float*�^ �Ŏ擾
	operator float* () { return &x; }

private:
};

//==================================================================
// 3D�x�N�g��
//	DirectX::SimpleMath::Vector3���p�����Ă��܂��B
//==================================================================
class RgVec3 : public DirectX::SimpleMath::Vector3
{
public:

	//====================================
	// �������E�Z�b�g
	//====================================

	// �R���X�g���N�^
	RgVec3() : DirectX::SimpleMath::Vector3() {}
	RgVec3(float fx, float fy, float fz) : DirectX::SimpleMath::Vector3(fx, fy, fz) {}
	RgVec3(const DirectX::XMFLOAT3& In) : DirectX::SimpleMath::Vector3(In) {}
	RgVec3(DirectX::FXMVECTOR V) : DirectX::SimpleMath::Vector3(V) {}

	// �Z�b�g
	void Set(float fx, float fy, float fz) {
		x = fx;
		y = fy;
		z = fz;
	}

	// Vector2(xy) �� z ���Z�b�g
	void Set(const DirectX::XMFLOAT2& v2, float fz) {
		x = v2.x;
		y = v2.y;
		z = fz;
	}

	//================================================
	//
	// �ϊ�
	//
	//================================================

	// 4x4�s��ŕϊ�����
	// �Em			�c �s��(4x4�Ƃ��Ĉ�����)
	void Transform(const RgMatrix& m);

	// 3x3�s��ŕϊ�����(��]�݂̂̕ϊ�)
	// �Em			�c �s��(3x3�Ƃ��Ĉ�����)
	void TransformNormal(const RgMatrix& m);

	//================================================
	//
	// �擾
	//
	//================================================

	// �擪�A�h���X�� float*�^ �Ŏ擾
	operator float* () { return &x; }

private:
};

//==================================================================
// 4D�x�N�g��
//	DirectX::SimpleMath::Vector4���p�����Ă��܂��B
//==================================================================
class RgVec4 : public DirectX::SimpleMath::Vector4
{
public:

	//====================================
	// �������E�Z�b�g
	//====================================

	// �R���X�g���N�^
	RgVec4() : DirectX::SimpleMath::Vector4() {}
	RgVec4(float fx, float fy, float fz, float fw) : DirectX::SimpleMath::Vector4(fx, fy, fz, fw) {}
	RgVec4(const DirectX::XMFLOAT4& In) : DirectX::SimpleMath::Vector4(In) {}
	RgVec4(DirectX::FXMVECTOR V) : DirectX::SimpleMath::Vector4(V) {}

	// �Z�b�g
	void Set(float fx, float fy, float fz, float fw) {
		x = fx;
		y = fy;
		z = fz;
		w = fw;
	}

	// Vector3(xyz) �� w ���Z�b�g
	void Set(const DirectX::XMFLOAT3& v3, float fw) {
		x = v3.x;
		y = v3.y;
		z = v3.z;
		w = fw;
	}

	//================================================
	//
	// �擾
	//
	//================================================

	// �擪�A�h���X�� float*�^ �Ŏ擾
	operator float* () { return &x; }

	// �R�����x�N�g���̎擾
	RgVec3 ToVec3() const { return { x, y, z }; }

private:
};


//==================================================================
// 4x4�̍s��N���X
//	DirectX::SimpleMath::Matrix���p�����Ă��܂��B
//==================================================================
class RgMatrix : public DirectX::SimpleMath::Matrix
{
public:

	//====================================
	// �������E�Z�b�g
	//====================================

	// �R���X�g���N�^
	RgMatrix() : DirectX::SimpleMath::Matrix() {}
	RgMatrix(const DirectX::XMFLOAT4X4& In) : DirectX::SimpleMath::Matrix(In) {}
	RgMatrix(DirectX::CXMMATRIX M) { DirectX::XMStoreFloat4x4(this, M); }


	//================================================
	//
	// �s��u�쐬�v�n
	//
	//================================================
	// �P�ʍs��
	void CreateIdentity() {
		*this = DirectX::XMMatrixIdentity();
	}

	// �ړ��s��쐬
	void CreateMove(float x, float y, float z) {
		*this = DirectX::SimpleMath::Matrix::CreateTranslation(x, y, z);
	}

	// �ړ��s��쐬
	void CreateMove(const RgVec3& v) {
		*this = DirectX::SimpleMath::Matrix::CreateTranslation(v);
	}

	// X��]�s��쐬
	// �Eangle	�c �p�x(�x)
	void CreateRotateX(float angle) {
		*this = DirectX::SimpleMath::Matrix::CreateRotationX(DirectX::XMConvertToRadians(angle));
	}

	// Y��]�s��쐬
	// �Eangle	�c �p�x(�x)
	void CreateRotateY(float angle) {
		*this = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(angle));
	}

	// Z��]�s��쐬
	// �Eangle	�c �p�x(�x)
	void CreateRotateZ(float angle) {
		*this = DirectX::SimpleMath::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(angle));
	}

	// �C�ӎ���]�s��쐬
	// �Eangle	�c �p�x(�x)
	void CreateRotateAxis(const RgVec3& axis, float angle) {
		*this = DirectX::SimpleMath::Matrix::CreateFromAxisAngle(axis, DirectX::XMConvertToRadians(angle));
	}

	// �g�k�s��쐬
	void CreateScale(float x, float y, float z) {
		*this = DirectX::SimpleMath::Matrix::CreateScale(x, y, z);
	}

	// �g�k�s��쐬
	void CreateScale(const RgVec3& v) {
		*this = DirectX::SimpleMath::Matrix::CreateScale(v);
	}

	// �������e�s��쐬
	// �EfovDegree		�c ����p(�x)
	// �EaspectRatio	�c �A�X�y�N�g��
	// �EfNear			�c �ŋߐڋ���
	// �EfFar			�c �ŉ�������
	void CreatePerspectiveFovLH(float fovDegree, float aspectRatio, float fNear, float fFar) {
		*this = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fovDegree), aspectRatio, fNear, fFar);
	}

	// ���ˉe�s��(�ˉe�s��)�쐬
	// �Ew				�c ��
	// �Eh				�c ����
	// �EfNear			�c �ŋߐڋ���
	// �EfFar			�c �ŉ�������
	void CreateOrthoLH(float w, float h, float fNear, float fFar) {
		*this = DirectX::XMMatrixOrthographicLH(w, h, fNear, fFar);
	}

	//================================================
	//
	// �s��u����v�n
	//
	//================================================

	// �������t�s��
	void Inverse() {
		Invert(*this);
	}

	// �t�s�񉻂�outM�ɓ����
	void Inverse(RgMatrix& outM) const {
		Invert(outM);
	}

	// �w��s����u���v�ɍ���
	void Multiply(const RgMatrix& mat) {
		DirectX::XMMATRIX M1 = DirectX::XMLoadFloat4x4(this);
		DirectX::XMMATRIX M2 = mat;
		*this = XMMatrixMultiply(M1, M2);
	}

	// �w��s����u��O�v�ɍ���
	void Multiply_Local(const RgMatrix& mat) {
		DirectX::XMMATRIX M1 = mat;
		DirectX::XMMATRIX M2 = DirectX::XMLoadFloat4x4(this);
		*this = XMMatrixMultiply(M1, M2);
	}

	//+++++++++++
	// �ړ�
	//+++++++++++

	// ���[���h�ړ�
	void Move(float x, float y, float z) {
		_41 += x;
		_42 += y;
		_43 += z;
	}

	// ���[���h�ړ�
	void Move(const RgVec3& v) {
		_41 += v.x;
		_42 += v.y;
		_43 += v.z;
	}

	// ���[�J���ړ�
	void Move_Local(float x, float y, float z) {
		Multiply_Local(DirectX::SimpleMath::Matrix::CreateTranslation(x, y, z));
	}

	// ���[�J���ړ�
	void Move_Local(const RgVec3& v) {
		Multiply_Local(DirectX::SimpleMath::Matrix::CreateTranslation(v));
	}

	// ���W���Z�b�g
	void SetPos(const RgVec3& pos) {
		_41 = pos.x;
		_42 = pos.y;
		_43 = pos.z;
	}

	// ���W���Z�b�g
	void SetPos(float x, float y, float z) {
		_41 = x;
		_42 = y;
		_43 = z;
	}

	//+++++++++++
	// ��]
	//+++++++++++

	// ���[���hX��]
	void RotateX(float angle) {
		Multiply(DirectX::SimpleMath::Matrix::CreateRotationX(DirectX::XMConvertToRadians(angle)));
	}

	// ���[���hY��]
	void RotateY(float angle) {
		Multiply(DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(angle)));
	}

	// ���[���hZ��]
	void RotateZ(float angle) {
		Multiply(DirectX::SimpleMath::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(angle)));
	}

	// ���[���h�w�莲��]
	void RotateAxis(const RgVec3& axis, float angle) {
		Multiply(DirectX::SimpleMath::Matrix::CreateFromAxisAngle(axis, DirectX::XMConvertToRadians(angle)));
	}

	// ���[�J��X��]
	void RotateX_Local(float angle) {
		Multiply_Local(DirectX::SimpleMath::Matrix::CreateRotationX(DirectX::XMConvertToRadians(angle)));
	}

	// ���[�J��Y��]
	void RotateY_Local(float angle) {
		Multiply_Local(DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(angle)));
	}

	// ���[�J��Z��]
	void RotateZ_Local(float angle) {
		Multiply_Local(DirectX::SimpleMath::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(angle)));
	}

	// ���[�J���w�莲��]
	void RotateAxis_Local(const RgVec3& axis, float angle) {
		Multiply_Local(DirectX::SimpleMath::Matrix::CreateFromAxisAngle(axis, DirectX::XMConvertToRadians(angle)));
	}


	// Z�����w������֌�����
	void LookTo(const RgVec3& lookDir, const RgVec3& up);
	// Z�����w��ʒu�֌�����
	void LookAt(const RgVec3& targetPos, const RgVec3& up);

	// �w��s��̉�](�Ɗg��)�����݂̂��Z�b�g����
	void SetRotation(const RgMatrix& m) {
		_11 = m._11;
		_12 = m._12;
		_13 = m._13;

		_21 = m._21;
		_22 = m._22;
		_23 = m._23;

		_31 = m._31;
		_32 = m._32;
		_33 = m._33;
	}

	//+++++++++++
	// �g��
	//+++++++++++

	// ���[���h�g�k
	void Scale(float x, float y, float z) {
		Multiply(DirectX::SimpleMath::Matrix::CreateScale(x, y, z));
	}

	// ���[���h�g�k
	void Scale(const RgVec3& v) {
		Multiply(DirectX::SimpleMath::Matrix::CreateScale(v));
	}

	// ���[�J���g�k
	void Scale_Local(float x, float y, float z) {
		Multiply_Local(DirectX::SimpleMath::Matrix::CreateScale(x, y, z));
	}

	// ���[�J���g�k
	void Scale_Local(const RgVec3& v) {
		Multiply_Local(DirectX::SimpleMath::Matrix::CreateScale(v));
	}

	// �g�听���𐳋K��
	void NormalizeScale() {
		// �e���𐳋K��
		((RgVec3&)_11).Normalize();
		((RgVec3&)_21).Normalize();
		((RgVec3&)_31).Normalize();
	}

	// �g�听�����Z�b�g
	void SetScale(const RgVec3& v) {
		NormalizeScale();
		((RgVec3&)_11) *= v.x;
		((RgVec3&)_21) *= v.y;
		((RgVec3&)_31) *= v.z;
	}

	// �g�听�����Z�b�g
	void SetScale(float x, float y, float z) {
		NormalizeScale();
		((RgVec3&)_11) *= x;
		((RgVec3&)_21) *= y;
		((RgVec3&)_31) *= z;
	}

	//================================================
	//
	// �擾
	//
	//================================================

	// ���W�擾
	RgVec3& GetPos() { return (RgVec3&)_41; }
	// ���W�擾(const ��)
	const RgVec3& GetPos() const { return (RgVec3&)_41; }

	// X���擾
	const RgVec3& GetXAxis() const { return (RgVec3&)_11; }
	// Y���擾
	const RgVec3& GetYAxis() const { return (RgVec3&)_21; }
	// Z���擾
	const RgVec3& GetZAxis() const { return (RgVec3&)_31; }

	// X���g�嗦�擾
	float GetXScale() const { return GetXAxis().Length(); }
	// Y���g�嗦�擾
	float GetYScale() const { return GetYAxis().Length(); }
	// Z���g�嗦�擾
	float GetZScale() const { return GetZAxis().Length(); }
	// X,Y,Z���̊g�嗦���擾
	RgVec3 GetScale() const {
		return { GetXScale(), GetYScale(), GetZScale() };
	}

	// �擪�A�h���X�� float*�^ �Ŏ擾
	operator float* () { return &_11; }
	operator const float* () const { return &_11; }
};


inline void RgVec3::Transform(const RgMatrix& m)
{
	*this = DirectX::SimpleMath::Vector3::Transform(*this, m);
}

inline void RgVec3::TransformNormal(const RgMatrix& m)
{
	*this = DirectX::SimpleMath::Vector3::TransformNormal(*this, m);
}