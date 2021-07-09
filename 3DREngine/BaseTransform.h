#ifndef BASETRANSFORM_H
#define BASETRANSFORM_H

#include "Global.h"
#include "BaseDrawable.h"

class CBaseTransform : public CBaseDrawable
{
public:
	DECLARE_CLASS( CBaseTransform, CBaseDrawable );

	CBaseTransform( const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );

	void SetPosition( const glm::vec3 &vecPosition );
	void SetRotation( const glm::quat &qRotation );
	void SetScale( const glm::vec3 &vecScale );

	void AddPosition( const glm::vec3 &vecPosition );
	void AddRotation( const glm::quat &qRotation );
	void AddScale( const glm::vec3 &vecScale );
	
	const glm::vec3 &GetPosition( void ) const;
	const glm::quat &GetRotation( void ) const;
	const glm::vec3 &GetScale( void ) const;

	void SetParent( CBaseTransform *pParent );
	void AddChild( CBaseTransform *pChild );
	void RemoveChild( CBaseTransform *pChild );

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

	bool HasTransformUpdated( void ) const;
	void ResetTransformUpdated( void );

private:
	glm::vec3 m_vecPosition;
	glm::quat m_qRotation;
	glm::vec3 m_vecScale;

	CBaseTransform *m_pParent;
	std::vector<CBaseTransform *> m_pChildren;

	bool m_bParentPosition;
	bool m_bParentRotation;
	bool m_bParentRotationPosition;
	bool m_bParentScale;
	bool m_bParentScalePosition;

	bool m_bTransformUpdated;
};

#endif // BASETRANSFORM_H