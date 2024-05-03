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

protected:
	void AddParentPosition( const glm::vec3 &vec3Position );
	void AddParentRotation( const glm::vec3 &vec3ParentPosition, const glm::quat &qParentRotation, const glm::quat &qRotationInverse, const glm::quat &qRotationDelta );
	void AddParentScale( const glm::vec3 &vec3Scale, const glm::vec3 &vec3ParentPosition, const glm::vec3 &vec3ParentScale );

private:
	CMonitoredValue<glm::vec3> m_vec3Position;
	CMonitoredValue<glm::quat> m_qRotation;
	CMonitoredValue<glm::vec3> m_vec3Scale;

protected:
	CHandle<CBaseTransform> m_hParent;
	std::vector<CHandle<CBaseTransform>> m_hChildren;
};

#endif // BASETRANSFORM_H