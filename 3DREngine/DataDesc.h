#ifndef DATADESC_H
#define DATADESC_H

#include "Global.h"
#include "FileManager.h"
#include "EntityManager.h"

// TODO: clean this up, maybe split into multiple files

// TODO: Consider adding FL_NONE (0)
#define FL_REQUIRED	(1<<0)
#define FL_SAVED	(1<<1)

class CBaseDataField
{
public:
	DECLARE_CLASS_NOBASE( CBaseDataField )

	CBaseDataField( const char *sName, unsigned int uiOffset, int iFlags );

	bool Save( void *pData ) const;
	bool Load( void *pData ) const;
	bool LoadText( void *pData, const CTextBlock *pTextBlock ) const;
	bool Link( void *pData ) const;

protected:
	virtual bool SaveInternal( void *pData ) const;
	virtual bool LoadInternal( void *pData ) const;
	virtual bool LoadTextInternal( void *pData, const CTextBlock *pTextBlock ) const;
	virtual bool LinkInternal( void *pData ) const;

protected:
	const char *m_sName;
	unsigned int m_uiOffset;
	int m_iFlags;
};

class CDataMap
{
public:
	DECLARE_CLASS_NOBASE( CDataMap )

	CDataMap();

	void AddDataField( CBaseDataField *pDataField );
	void SetBaseMap( CDataMap *pBaseMap );

	bool Save( void *pData ) const;
	bool Load( void *pData ) const;
	bool LoadText( void *pData, const CTextBlock *pTextBlock ) const;
	bool Link( void *pData ) const;

private:
	std::vector<CBaseDataField *> m_pDataFields;
	CDataMap *m_pBaseMap;
};

template <class T> bool UTIL_SaveData( T *pData )
{
	return pData->GetDataMap()->Save( pData );
}

template <class T> bool UTIL_LoadData( T *pData )
{
	return pData->GetDataMap()->Load( pData );
}

template <class T> bool UTIL_LoadTextData( T *pData, const CTextBlock *pTextBlock )
{
	return pData->GetDataMap()->LoadText( pData, pTextBlock );
}

template <class T> bool UTIL_LinkData( T *pData )
{
	return pData->GetDataMap()->Link( pData );
}

#define DEFINE_FIELDTYPE( templatename, name, basename ) \
	template <class templatename> class name : public basename \
	{ \
	public: \
		DECLARE_CLASS( name<templatename>, basename ) \
		name( const char *sName, unsigned int uiOffset, int iFlags ) : BaseClass( sName, uiOffset, iFlags ) { }

#define DEFINE_FIELDTYPE_SAVE( dataname ) \
		virtual bool SaveInternal( void *dataname ) const

#define DEFINE_FIELDTYPE_LOAD( dataname ) \
		virtual bool LoadInternal( void *dataname ) const

#define DEFINE_FIELDTYPE_LOAD_TEXT( dataname, textblockname ) \
		virtual bool LoadTextInternal( void *dataname, const CTextBlock *textblockname ) const

#define DEFINE_FIELDTYPE_LINK( dataname ) \
		virtual bool LinkInternal( void *dataname ) const

#define END_FIELDTYPE() \
	};

#define DEFINE_FIELDTYPE_NOBASE( templatename, name )			DEFINE_FIELDTYPE( templatename, name, CBaseDataField )

#define DEFINE_FIELD( fieldtype, type, variable, name, flags ) \
	static fieldtype<type> variable##_dataField( name, (unsigned int)offsetof( classNameTypedef, variable ), flags ); \
	classNameTypedef::m_dmDataMap.AddDataField( &variable##_dataField );

#define DECLARE_DATADESC() \
	static CDataMap m_dmDataMap; \
	static CDataMap *GetBaseMap( void ); \
	virtual CDataMap *GetDataMap( void ); \
	friend CDataMap *DataMapAccess( ThisClass * ); \
	friend CDataMap *DataMapInit( ThisClass * );

#define DEFINE_DATADESC( className ) \
	CDataMap className::m_dmDataMap; \
	CDataMap *className::GetBaseMap( void ) { return DataMapAccess( (className::BaseClass *)NULL ); } \
	CDataMap *className::GetDataMap( void ) { return &m_dmDataMap; } \
	DEFINE_DATADESC_GUTS( className )

#define DEFINE_DATADESC_NOBASE( className ) \
	CDataMap className::m_dmDataMap; \
	CDataMap *className::GetBaseMap( void ) { return NULL; } \
	CDataMap *className::GetDataMap( void ) { return &m_dmDataMap; } \
	DEFINE_DATADESC_GUTS( className )

#define DEFINE_DATADESC_GUTS( className ) \
	CDataMap *DataMapAccess( className * ); \
	CDataMap *DataMapInit( className * ); \
	namespace className##_DataDescInit \
	{ \
		CDataMap *g_pDataMapHolder = DataMapInit( (className *)NULL ); \
	} \
	\
	CDataMap *DataMapAccess( className * ) \
	{ \
		return &className::m_dmDataMap; \
	} \
	CDataMap *DataMapInit( className * ) \
	{ \
		typedef className classNameTypedef; \
		className::m_dmDataMap.SetBaseMap( className::GetBaseMap() ); \

#define END_DATADESC() \
		return &classNameTypedef::m_dmDataMap; \
	}

#define GET_DATA_ADDRESS( data, offset, type )	((type *)((char *)data + offset))
#define GET_DATA( data, offset, type )			(*(GET_DATA_ADDRESS( data, offset, type )))

// TODO: make a macro for m_uiOffset and m_sName

DEFINE_FIELDTYPE_NOBASE( T, DataField )

	DEFINE_FIELDTYPE_SAVE( pData )
	{
		return pFileManager->Write( GET_DATA( pData, m_uiOffset, T ) );
	}

	DEFINE_FIELDTYPE_LOAD( pData )
	{
		return pFileManager->Read( GET_DATA( pData, m_uiOffset, T ) );
	}

	DEFINE_FIELDTYPE_LOAD_TEXT( pData, pTextBlock )
	{
		return m_sName && pTextBlock->GetValue( GET_DATA( pData, m_uiOffset, T ), 1, m_sName );
	}

END_FIELDTYPE()

DEFINE_FIELDTYPE( T, LinkedDataField, DataField<T> )

	DEFINE_FIELDTYPE_LINK( pData )
	{
		T &tData = GET_DATA( pData, CBaseDataField::m_uiOffset, T );
		return tData.Link();
	}

END_FIELDTYPE()

DEFINE_FIELDTYPE_NOBASE( T, EmbeddedDataField )

	DEFINE_FIELDTYPE_SAVE( pData )
	{
		return UTIL_SaveData( GET_DATA( pData, m_uiOffset, T * ) );
	}

	DEFINE_FIELDTYPE_LOAD( pData )
	{
		return UTIL_LoadData( GET_DATA( pData, m_uiOffset, T * ) );
	}

	DEFINE_FIELDTYPE_LOAD_TEXT( pData, pTextBlock )
	{
		if (!m_sName)
			return false;

		CTextBlock *pEmbeddedTextBlock;
		if (!pTextBlock->GetValue( pEmbeddedTextBlock, 1, m_sName ))
			return false;

		return UTIL_LoadTextData( GET_DATA( pData, m_uiOffset, T * ), pEmbeddedTextBlock);
	}

END_FIELDTYPE()

DEFINE_FIELDTYPE( T, LinkedEmbeddedDataField, EmbeddedDataField<T> )

	DEFINE_FIELDTYPE_LINK( pData )
	{
		return GET_DATA( pData, CBaseDataField::m_uiOffset, T * )->Link();
	}

END_FIELDTYPE()

DEFINE_FIELDTYPE_NOBASE( T, VectorDataField )

	DEFINE_FIELDTYPE_SAVE( pData )
	{
		std::vector<T> &vecData = GET_DATA( pData, m_uiOffset, std::vector<T> );

		unsigned int uiSize = (unsigned int)vecData.size();
		if (!pFileManager->Write( uiSize ))
			return false;

		for (unsigned int i = 0; i < uiSize; i++)
		{
			if (!pFileManager->Write( vecData[i] ))
				return false;
		}

		return true;
	}

	DEFINE_FIELDTYPE_LOAD( pData )
	{
		unsigned int uiSize;
		if (!pFileManager->Read( uiSize ))
			return false;

		std::vector<T> &vecData = GET_DATA( pData, CBaseDataField::m_uiOffset, std::vector<T> );
		vecData.resize( uiSize );
		for (unsigned int i = 0; i < uiSize; i++)
		{
			if (!pFileManager->Read( vecData[i] ))
				return false;
		}

		return true;
	}

	DEFINE_FIELDTYPE_LOAD_TEXT( pData, pTextBlock )
	{
		if (!m_sName)
			return false;

		CTextLine *pTextLine;
		if (!pTextBlock->GetValue( pTextLine, 1, m_sName ))
			return false;

		std::vector<T> &vecData = GET_DATA( pData, m_uiOffset, std::vector<T> );
		vecData.resize( pTextLine->GetTextItemCount() );
		for (unsigned int i = 0; i < vecData.size(); i++)
		{
			if (!pTextLine->GetValue( vecData[i], i ))
				return false;
		}

		return true;
	}

END_FIELDTYPE()

DEFINE_FIELDTYPE( T, LinkedVectorDataField, VectorDataField<T> )

	DEFINE_FIELDTYPE_LINK( pData )
	{
		std::vector<T> &vecData = GET_DATA( pData, CBaseDataField::m_uiOffset, std::vector<T> );
		if (vecData.empty())
			return false;

		for (unsigned int i = 0; i < vecData.size(); i++)
		{
			if (!vecData[i].Link())
				return false;
		}

		return true;
	}

END_FIELDTYPE()

DEFINE_FIELDTYPE_NOBASE( T, EmbeddedVectorDataField )

	DEFINE_FIELDTYPE_SAVE( pData )
	{
		std::vector<T *> &vecData = GET_DATA( pData, m_uiOffset, std::vector<T *> );
		unsigned int uiSize = (unsigned int)vecData.size();
		if (!pFileManager->Write( uiSize ))
			return false;

		for (unsigned int i = 0; i < uiSize; i++)
		{
			if (!UTIL_SaveData( vecData[i] ))
				return false;
		}

		return true;
	}

	DEFINE_FIELDTYPE_LOAD( pData )
	{
		unsigned int uiSize;
		if (!pFileManager->Read( uiSize ))
			return false;

		std::vector<T *> &vecData = GET_DATA( pData, m_uiOffset, std::vector<T *> );
		vecData.resize( uiSize );
		for (unsigned int i = 0; i < uiSize; i++)
		{
			if (!UTIL_LoadData( vecData[i] ))
				return false;
		}

		return true;
	}

	DEFINE_FIELDTYPE_LOAD_TEXT( pData, pTextBlock )
	{
		if (!m_sName)
			return false;

		CTextLine *pTextLine;
		if (!pTextBlock->GetValue( pTextLine, 1, m_sName ))
			return false;

		std::vector<T *> &vecData = GET_DATA( pData, m_uiOffset, std::vector<T *> );
		vecData.resize( pTextLine->GetTextItemCount() );
		for (unsigned int i = 0; i < vecData.size(); i++)
		{
			CTextBlock *pEmbeddedTextBlock;
			if (!pTextLine->GetValue( pEmbeddedTextBlock, i ))
				return false;

			if (!UTIL_LoadData( vecData[i], pEmbeddedTextBlock ))
				return false;
		}

		return true;
	}

END_FIELDTYPE()

DEFINE_FIELDTYPE( T, LinkedEmbeddedVectorDataField, EmbeddedVectorDataField<T> )

	DEFINE_FIELDTYPE_LINK( pData )
	{
		std::vector<T *> &vecData = GET_DATA( pData, CBaseDataField::m_uiOffset, std::vector<T *> );
		if (vecData.empty())
			return false;

		for (unsigned int i = 0; i < vecData.size(); i++)
		{
			if (!vecData[i]->Link())
				return false;
		}

		return true;
	}

END_FIELDTYPE()

DEFINE_FIELDTYPE_NOBASE( T, FlagDataField )

	DEFINE_FIELDTYPE_SAVE( pData )
	{
		return pFileManager->Write( GET_DATA( pData, m_uiOffset, T ) );
	}

	DEFINE_FIELDTYPE_LOAD( pData )
	{
		return pFileManager->Read( GET_DATA( pData, m_uiOffset, T ) );
	}

	DEFINE_FIELDTYPE_LOAD_TEXT( pData, pTextBlock )
	{
		if (!m_sName)
			return false;

		CTextLine *pTextLine;
		if (!pTextBlock->GetValue( pTextLine, 1, m_sName ))
			return false;

		T &tData = GET_DATA( pData, m_uiOffset, T );

		for (unsigned int i = 0; i < pTextLine->GetTextItemCount(); i++)
		{
			const char *sKey;
			if (!pTextLine->GetValue( sKey, i ))
				return false;

			bool bNot = false;
			if (*sKey == '!')
			{
				bNot = true;
				sKey++;
			}

			int iFlag = pEntityManager->GetFlag( sKey );
			if (iFlag == -1)
				return false;

			if (bNot)
				tData &= ~iFlag;
			else
				tData |= iFlag;
		}
		
		return true;
	}

END_FIELDTYPE()

#endif // DATADESC_H