#pragma once

//==================================================================
//
// 算術クラスをまとめたもの
//
//==================================================================

class RgVec2;
class RgVec3;
class RgVec4;
class RgMatrix;


//==================================================================
// 2Dベクトル
//	DirectX::SimpleMath::Vector2を継承しています。
//==================================================================
class RgVec2 : public DirectX::SimpleMath::Vector2
{
public:

	//====================================
	// 初期化・セット
	//====================================

	// コンストラクタ
	RgVec2() : DirectX::SimpleMath::Vector2() {}
	RgVec2(float fx, float fy) : DirectX::SimpleMath::Vector2(fx, fy){}
	RgVec2(const DirectX::XMFLOAT2& In) : DirectX::SimpleMath::Vector2(In) {}
	RgVec2(DirectX::FXMVECTOR V) : DirectX::SimpleMath::Vector2(V) {}

	// セット
	void Set(float fx, float fy) {
		x = fx;
		y = fy;
	}

	//================================================
	//
	// 取得
	//
	//================================================
	// 先頭アドレスを float*型 で取得
	operator float* () { return &x; }

private:
};

//==================================================================
// 3Dベクトル
//	DirectX::SimpleMath::Vector3を継承しています。
//==================================================================
class RgVec3 : public DirectX::SimpleMath::Vector3
{
public:

	//====================================
	// 初期化・セット
	//====================================

	// コンストラクタ
	RgVec3() : DirectX::SimpleMath::Vector3() {}
	RgVec3(float fx, float fy, float fz) : DirectX::SimpleMath::Vector3(fx, fy, fz) {}
	RgVec3(const DirectX::XMFLOAT3& In) : DirectX::SimpleMath::Vector3(In) {}
	RgVec3(DirectX::FXMVECTOR V) : DirectX::SimpleMath::Vector3(V) {}

	// セット
	void Set(float fx, float fy, float fz) {
		x = fx;
		y = fy;
		z = fz;
	}

	// Vector2(xy) と z をセット
	void Set(const DirectX::XMFLOAT2& v2, float fz) {
		x = v2.x;
		y = v2.y;
		z = fz;
	}

	//================================================
	//
	// 変換
	//
	//================================================

	// 4x4行列で変換する
	// ・m			… 行列(4x4として扱われる)
	void Transform(const RgMatrix& m);

	// 3x3行列で変換する(回転のみの変換)
	// ・m			… 行列(3x3として扱われる)
	void TransformNormal(const RgMatrix& m);

	//================================================
	//
	// 取得
	//
	//================================================

	// 先頭アドレスを float*型 で取得
	operator float* () { return &x; }

private:
};

//==================================================================
// 4Dベクトル
//	DirectX::SimpleMath::Vector4を継承しています。
//==================================================================
class RgVec4 : public DirectX::SimpleMath::Vector4
{
public:

	//====================================
	// 初期化・セット
	//====================================

	// コンストラクタ
	RgVec4() : DirectX::SimpleMath::Vector4() {}
	RgVec4(float fx, float fy, float fz, float fw) : DirectX::SimpleMath::Vector4(fx, fy, fz, fw) {}
	RgVec4(const DirectX::XMFLOAT4& In) : DirectX::SimpleMath::Vector4(In) {}
	RgVec4(DirectX::FXMVECTOR V) : DirectX::SimpleMath::Vector4(V) {}

	// セット
	void Set(float fx, float fy, float fz, float fw) {
		x = fx;
		y = fy;
		z = fz;
		w = fw;
	}

	// Vector3(xyz) と w をセット
	void Set(const DirectX::XMFLOAT3& v3, float fw) {
		x = v3.x;
		y = v3.y;
		z = v3.z;
		w = fw;
	}

	//================================================
	//
	// 取得
	//
	//================================================

	// 先頭アドレスを float*型 で取得
	operator float* () { return &x; }

	// ３次元ベクトルの取得
	RgVec3 ToVec3() const { return { x, y, z }; }

private:
};


//==================================================================
// 4x4の行列クラス
//	DirectX::SimpleMath::Matrixを継承しています。
//==================================================================
class RgMatrix : public DirectX::SimpleMath::Matrix
{
public:

	//====================================
	// 初期化・セット
	//====================================

	// コンストラクタ
	RgMatrix() : DirectX::SimpleMath::Matrix() {}
	RgMatrix(const DirectX::XMFLOAT4X4& In) : DirectX::SimpleMath::Matrix(In) {}
	RgMatrix(DirectX::CXMMATRIX M) { DirectX::XMStoreFloat4x4(this, M); }


	//================================================
	//
	// 行列「作成」系
	//
	//================================================
	// 単位行列
	void CreateIdentity() {
		*this = DirectX::XMMatrixIdentity();
	}

	// 移動行列作成
	void CreateMove(float x, float y, float z) {
		*this = DirectX::SimpleMath::Matrix::CreateTranslation(x, y, z);
	}

	// 移動行列作成
	void CreateMove(const RgVec3& v) {
		*this = DirectX::SimpleMath::Matrix::CreateTranslation(v);
	}

	// X回転行列作成
	// ・angle	… 角度(度)
	void CreateRotateX(float angle) {
		*this = DirectX::SimpleMath::Matrix::CreateRotationX(DirectX::XMConvertToRadians(angle));
	}

	// Y回転行列作成
	// ・angle	… 角度(度)
	void CreateRotateY(float angle) {
		*this = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(angle));
	}

	// Z回転行列作成
	// ・angle	… 角度(度)
	void CreateRotateZ(float angle) {
		*this = DirectX::SimpleMath::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(angle));
	}

	// 任意軸回転行列作成
	// ・angle	… 角度(度)
	void CreateRotateAxis(const RgVec3& axis, float angle) {
		*this = DirectX::SimpleMath::Matrix::CreateFromAxisAngle(axis, DirectX::XMConvertToRadians(angle));
	}

	// 拡縮行列作成
	void CreateScale(float x, float y, float z) {
		*this = DirectX::SimpleMath::Matrix::CreateScale(x, y, z);
	}

	// 拡縮行列作成
	void CreateScale(const RgVec3& v) {
		*this = DirectX::SimpleMath::Matrix::CreateScale(v);
	}

	// 透視投影行列作成
	// ・fovDegree		… 視野角(度)
	// ・aspectRatio	… アスペクト比
	// ・fNear			… 最近接距離
	// ・fFar			… 最遠方距離
	void CreatePerspectiveFovLH(float fovDegree, float aspectRatio, float fNear, float fFar) {
		*this = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fovDegree), aspectRatio, fNear, fFar);
	}

	// 正射影行列(射影行列)作成
	// ・w				… 幅
	// ・h				… 高さ
	// ・fNear			… 最近接距離
	// ・fFar			… 最遠方距離
	void CreateOrthoLH(float w, float h, float fNear, float fFar) {
		*this = DirectX::XMMatrixOrthographicLH(w, h, fNear, fFar);
	}

	//================================================
	//
	// 行列「操作」系
	//
	//================================================

	// 自分を逆行列化
	void Inverse() {
		Invert(*this);
	}

	// 逆行列化をoutMに入れる
	void Inverse(RgMatrix& outM) const {
		Invert(outM);
	}

	// 指定行列を「後ろ」に合成
	void Multiply(const RgMatrix& mat) {
		DirectX::XMMATRIX M1 = DirectX::XMLoadFloat4x4(this);
		DirectX::XMMATRIX M2 = mat;
		*this = XMMatrixMultiply(M1, M2);
	}

	// 指定行列を「手前」に合成
	void Multiply_Local(const RgMatrix& mat) {
		DirectX::XMMATRIX M1 = mat;
		DirectX::XMMATRIX M2 = DirectX::XMLoadFloat4x4(this);
		*this = XMMatrixMultiply(M1, M2);
	}

	//+++++++++++
	// 移動
	//+++++++++++

	// ワールド移動
	void Move(float x, float y, float z) {
		_41 += x;
		_42 += y;
		_43 += z;
	}

	// ワールド移動
	void Move(const RgVec3& v) {
		_41 += v.x;
		_42 += v.y;
		_43 += v.z;
	}

	// ローカル移動
	void Move_Local(float x, float y, float z) {
		Multiply_Local(DirectX::SimpleMath::Matrix::CreateTranslation(x, y, z));
	}

	// ローカル移動
	void Move_Local(const RgVec3& v) {
		Multiply_Local(DirectX::SimpleMath::Matrix::CreateTranslation(v));
	}

	// 座標をセット
	void SetPos(const RgVec3& pos) {
		_41 = pos.x;
		_42 = pos.y;
		_43 = pos.z;
	}

	// 座標をセット
	void SetPos(float x, float y, float z) {
		_41 = x;
		_42 = y;
		_43 = z;
	}

	//+++++++++++
	// 回転
	//+++++++++++

	// ワールドX回転
	void RotateX(float angle) {
		Multiply(DirectX::SimpleMath::Matrix::CreateRotationX(DirectX::XMConvertToRadians(angle)));
	}

	// ワールドY回転
	void RotateY(float angle) {
		Multiply(DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(angle)));
	}

	// ワールドZ回転
	void RotateZ(float angle) {
		Multiply(DirectX::SimpleMath::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(angle)));
	}

	// ワールド指定軸回転
	void RotateAxis(const RgVec3& axis, float angle) {
		Multiply(DirectX::SimpleMath::Matrix::CreateFromAxisAngle(axis, DirectX::XMConvertToRadians(angle)));
	}

	// ローカルX回転
	void RotateX_Local(float angle) {
		Multiply_Local(DirectX::SimpleMath::Matrix::CreateRotationX(DirectX::XMConvertToRadians(angle)));
	}

	// ローカルY回転
	void RotateY_Local(float angle) {
		Multiply_Local(DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(angle)));
	}

	// ローカルZ回転
	void RotateZ_Local(float angle) {
		Multiply_Local(DirectX::SimpleMath::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(angle)));
	}

	// ローカル指定軸回転
	void RotateAxis_Local(const RgVec3& axis, float angle) {
		Multiply_Local(DirectX::SimpleMath::Matrix::CreateFromAxisAngle(axis, DirectX::XMConvertToRadians(angle)));
	}


	// Z軸を指定方向へ向ける
	void LookTo(const RgVec3& lookDir, const RgVec3& up);
	// Z軸を指定位置へ向ける
	void LookAt(const RgVec3& targetPos, const RgVec3& up);

	// 指定行列の回転(と拡大)部分のみをセットする
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
	// 拡大
	//+++++++++++

	// ワールド拡縮
	void Scale(float x, float y, float z) {
		Multiply(DirectX::SimpleMath::Matrix::CreateScale(x, y, z));
	}

	// ワールド拡縮
	void Scale(const RgVec3& v) {
		Multiply(DirectX::SimpleMath::Matrix::CreateScale(v));
	}

	// ローカル拡縮
	void Scale_Local(float x, float y, float z) {
		Multiply_Local(DirectX::SimpleMath::Matrix::CreateScale(x, y, z));
	}

	// ローカル拡縮
	void Scale_Local(const RgVec3& v) {
		Multiply_Local(DirectX::SimpleMath::Matrix::CreateScale(v));
	}

	// 拡大成分を正規化
	void NormalizeScale() {
		// 各軸を正規化
		((RgVec3&)_11).Normalize();
		((RgVec3&)_21).Normalize();
		((RgVec3&)_31).Normalize();
	}

	// 拡大成分をセット
	void SetScale(const RgVec3& v) {
		NormalizeScale();
		((RgVec3&)_11) *= v.x;
		((RgVec3&)_21) *= v.y;
		((RgVec3&)_31) *= v.z;
	}

	// 拡大成分をセット
	void SetScale(float x, float y, float z) {
		NormalizeScale();
		((RgVec3&)_11) *= x;
		((RgVec3&)_21) *= y;
		((RgVec3&)_31) *= z;
	}

	//================================================
	//
	// 取得
	//
	//================================================

	// 座標取得
	RgVec3& GetPos() { return (RgVec3&)_41; }
	// 座標取得(const 版)
	const RgVec3& GetPos() const { return (RgVec3&)_41; }

	// X軸取得
	const RgVec3& GetXAxis() const { return (RgVec3&)_11; }
	// Y軸取得
	const RgVec3& GetYAxis() const { return (RgVec3&)_21; }
	// Z軸取得
	const RgVec3& GetZAxis() const { return (RgVec3&)_31; }

	// X軸拡大率取得
	float GetXScale() const { return GetXAxis().Length(); }
	// Y軸拡大率取得
	float GetYScale() const { return GetYAxis().Length(); }
	// Z軸拡大率取得
	float GetZScale() const { return GetZAxis().Length(); }
	// X,Y,Z軸の拡大率を取得
	RgVec3 GetScale() const {
		return { GetXScale(), GetYScale(), GetZScale() };
	}

	// 先頭アドレスを float*型 で取得
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