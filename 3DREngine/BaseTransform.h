#ifndef BASETRANSFORM_H
#define BASETRANSFORM_H

#include "Global.h"
#include "BaseDrawable.h"

class CBaseTransform : public CBaseDrawable
{
public:
	DECLARE_CLASS( CBaseTransform, CBaseDrawable );

	CBaseTransform();

	virtual void Init( void );

	void SetPosition( const glm::vec3 &vec3Position );
	void SetRotation( const glm::quat &qRotation );
	void SetScale( const glm::vec3 &vec3Scale );

	void AddPosition( const glm::vec3 &vec3Position );
	void AddRotation( const glm::quat &qRotation );
	void AddScale( const glm::vec3 &vec3Scale );
	
	const glm::vec3 &GetPosition( void ) const;
	const glm::quat &GetRotation( void ) const;
	const glm::vec3 &GetScale( void ) const;

	void SetParent( CBaseTransform *pParent );
	void AddChild( CBaseTransform *pChild );
	void RemoveChild( CBaseTransform *pChild );

	CBaseTransform *GetParent( void ) const;
	const std::vector<CBaseTransform *> &GetChildren( void ) const;

	void SetParentPosition( bool bParentPosition );
	void SetParentRotation( bool bParentRotation );
	void SetParentRotationPosition( bool bParentRotationPosition );
	void SetParentScale( bool bParentScale );
	void SetParentScalePosition( bool bParentScalePosition );

	bool ParentPosition( void ) const;
	bool ParentRotation( void ) const;
	bool ParentRotationPosition( void ) const;
	bool ParentScale( void ) const;
	bool ParentScalePosition( void ) const;

	bool PositionUpdated( void ) const;
	bool RotationUpdated( void ) const;
	bool ScaleUpdated( void ) const;

private:
	glm::vec3 m_vec3Position;
	glm::quat m_qRotation;
	glm::vec3 m_vec3Scale;

	CBaseTransform *m_pParent;
	std::vector<CBaseTransform *> m_pChildren;

	bool m_bParentPosition;
	bool m_bParentRotation;
	bool m_bParentRotationPosition;
	bool m_bParentScale;
	bool m_bParentScalePosition;

	unsigned int m_uiLastFramePositionUpdated;
	unsigned int m_uiLastFrameRotationUpdated;
	unsigned int m_uiLastFrameScaleUpdated;
};

#endif // BASETRANSFORM_H