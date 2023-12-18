#ifndef BASETRANSFORM_H
#define BASETRANSFORM_H

#include "Global.h"
#include "BaseDrawable.h"

DEFINE_ENTITY_FLAG( fl_parentposition, 1 )
DEFINE_ENTITY_FLAG( fl_parentrotation, 2 )
DEFINE_ENTITY_FLAG( fl_parentscale, 3 )

class CBaseTransform : public CBaseDrawable
{
public:
	DECLARE_CLASS( CBaseTransform, CBaseDrawable )

	DECLARE_DATADESC()

	CBaseTransform();

	virtual bool Init( void );

	virtual void PostThink( void );

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

	bool PositionUpdated( void ) const;
	bool RotationUpdated( void ) const;
	bool ScaleUpdated( void ) const;

private:
	void MarkPositionUpdated( void );
	void MarkRotationUpdated( void );
	void MarkScaleUpdated( void );

private:
	glm::vec3 m_vec3Position;
	glm::quat m_qRotation;
	glm::vec3 m_vec3Scale;

	CHandle<CBaseTransform> m_hParent;
	std::vector<CHandle<CBaseTransform>> m_hChildren;

	unsigned long m_ulLastFramePositionUpdated; // TODO: consider whether these should be saved
	unsigned long m_ulLastFrameRotationUpdated;
	unsigned long m_ulLastFrameScaleUpdated;
};

#endif // BASETRANSFORM_H