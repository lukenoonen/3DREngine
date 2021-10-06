#ifndef BASEHUDTRANSFORM_H
#define BASEHUDTRANSFORM_H

#include "Global.h"
#include "BaseDrawable.h"

class CBaseHUDTransform : public CBaseDrawable
{
public:
	DECLARE_CLASS( CBaseHUDTransform, CBaseDrawable );

	CBaseHUDTransform( const glm::vec2 &vecPosition, const glm::vec2 &vecScale, float flPriority, bool bShouldDraw, bool bActive );

	void SetPosition( const glm::vec2 &vecPosition );
	void SetScale( const glm::vec2 &vecScale );
	void SetPriority( float flPriority );

	void AddPosition( const glm::vec2 &vecPosition );
	void AddScale( const glm::vec2 &vecScale );
	void AddPriority( float flPriority );

	const glm::vec2 &GetPosition( void ) const;
	const glm::vec2 &GetScale( void ) const;
	float GetPriority( void ) const;

	void SetParent( CBaseHUDTransform *pParent );
	void AddChild( CBaseHUDTransform *pChild );
	void RemoveChild( CBaseHUDTransform *pChild );

	void SetParentPosition( bool bParentPosition );
	void SetParentScale( bool bParentScale );
	void SetParentScalePosition( bool bParentScalePosition );
	void SetParentPriority( bool bParentPriority );

	bool ParentPosition( void ) const;
	bool ParentScale( void ) const;
	bool ParentScalePosition( void ) const;
	bool ParentPriority( void ) const;

private:
	glm::vec2 m_vecPosition;
	glm::vec2 m_vecScale;
	float m_flPriority;

	CBaseHUDTransform *m_pParent;
	std::vector<CBaseHUDTransform *> m_pChildren;

	bool m_bParentPosition;
	bool m_bParentScale;
	bool m_bParentScalePosition;
	bool m_bParentPriority;
};

#endif // BASEHUDTRANSFORM_H