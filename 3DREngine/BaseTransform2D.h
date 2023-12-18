#ifndef BASETRANSFORM2D_H
#define BASETRANSFORM2D_H

#include "Global.h"
#include "BaseTransform.h"

class CBaseTransform2D : public CBaseTransform
{
public:
	DECLARE_CLASS( CBaseTransform2D, CBaseTransform )

	DECLARE_DATADESC()

	CBaseTransform2D();

	virtual void PostThink( void );

	void SetPosition( const glm::vec2 &vec2Position );
	void SetRotation( float flRotation );
	void SetScale( const glm::vec2 &vec2Scale );

	void AddPosition( const glm::vec2 &vec2Position );
	void AddRotation( float flRotation );
	void AddScale( const glm::vec2 &vec2Scale );

	const glm::vec2 &GetPosition( void ) const;
	float GetRotation( void ) const;
	const glm::vec2 &GetScale( void ) const;

	void SetParent( CBaseTransform2D *pParent );
	void AddChild( CBaseTransform2D *pChild );
	void RemoveChild( CBaseTransform2D *pChild );

private:
	glm::vec2 m_vec2Position;
	float m_flRotation;
	glm::vec2 m_vec2Scale;

	CHandle<CBaseTransform2D> m_hParent;
	std::vector<CHandle<CBaseTransform2D>> m_hChildren;
};

#endif // BASETRANSFORM2D_H