#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Global.h"
#include "BaseAsset.h"

class CGeometry : public CBaseAsset
{
public:
	DECLARE_CLASS( CGeometry, CBaseAsset )

	CGeometry();

	virtual bool Init( void );
	virtual bool Exit( void );

	void Draw( void ); // TODO: clarify language between "draw", "render", etc

private:
	std::vector<SVertex> m_verVertices;
	std::vector<unsigned int> m_uiIndices;

	GLuint m_glVAO;
	GLuint m_glVBO;
	GLuint m_glEBO;
};

#endif // GEOMETRY_H