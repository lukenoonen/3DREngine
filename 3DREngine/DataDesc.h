#ifndef DATADESC_H
#define DATADESC_H

#include "Global.h"
#include "File.h"
#include "TextReader.h"

// TODO: clean this up, maybe split into multiple files

class CBaseDataField
{
public:
	DECLARE_CLASS_NOBASE( CBaseDataField )

	CBaseDataField( const char *sName, unsigned int uiOffset );

	virtual bool Save( void *pData, CFile *pFile );
	virtual bool Load( void *pData, CFile *pFile );
	virtual bool Load( void *pData, CTextBlock *pTextBlock );

protected:
	const char *m_sName;
	unsigned int m_uiOffset;
};

class CDataMap
{
public:
	DECLARE_CLASS_NOBASE( CDataMap )

	CDataMap();

	void AddDataField( CBaseDataField *pDataField );
	void SetBaseMap( CDataMap *pBaseMap );

	bool Save( void *pData, CFile *pFile );
	bool Load( void *pData, CFile *pFile );
	bool Load( void *pData, CTextBlock *pTextBlock );

private:
	std::vector<CBaseDataField *> m_pDataFields;
	CDataMap *m_pBaseMap;
};

template <typename T> void UTIL_DataMapAccess( CDataMap **p ) // TODO: what should/should not be inline?
{
	*p = &T::m_dmDataMap;
}

template <typename T> bool UTIL_SaveData( T *pData, CFile *pFile )
{
	return pData->GetDataMap()->Save( pData, pFile );
}

template <typename T> bool UTIL_LoadData( T *pData, CFile *pFile )
{
	return pData->GetDataMap()->Load( pData, pFile );
}

template <typename T> bool UTIL_LoadData( T *pData, CTextBlock *pTextBlock )
{
	return pData->GetDataMap()->Load( pData, pTextBlock );
}

#define DEFINE_FIELDTYPE( name ) \
	template <class T> class name : public CBaseDataField \
	{ \
	public: \
		DECLARE_CLASS( name<T>, CBaseDataField ) \
		name( const char *sName, unsigned int uiOffset ) : BaseClass( sName, uiOffset ) { }

#define DEFINE_FIELDTYPE_SAVE( dataname, filename ) \
		virtual bool Save( void *dataname, CFile *filename )

#define DEFINE_FIELDTYPE_LOAD( dataname, filename ) \
		virtual bool Load( void *dataname, CFile *filename )

#define DEFINE_FIELDTYPE_LOAD_TEXT( dataname, textblockname ) \
		virtual bool Load( void *dataname, CTextBlock *textblockname )

#define END_FIELDTYPE() \
	};

#define DEFINE_FIELD( fieldtype, type, variable, name ) \
	static fieldtype<type> variable##_dataField( name, (unsigned int)offsetof( classNameTypedef, variable ) ); \
	classNameTypedef::m_dmDataMap.AddDataField( &variable##_dataField );

#define DECLARE_DATADESC() \
	static CDataMap m_dmDataMap; \
	static CDataMap *GetBaseMap( void ); \
	virtual CDataMap *GetDataMap( void ); \
	template <typename T> friend void UTIL_DataMapAccess( CDataMap **p ); \
	template <typename T> friend CDataMap *DataMapInit( T * );

#define DEFINE_DATADESC( className ) \
	CDataMap className::m_dmDataMap; \
	CDataMap *className::GetBaseMap( void ) { CDataMap *pResult; UTIL_DataMapAccess<BaseClass>( &pResult ); return pResult; } \
	CDataMap *className::GetDataMap( void ) { return &m_dmDataMap; } \
	DEFINE_DATADESC_GUTS( className )

#define DEFINE_DATADESC_NOBASE( className ) \
	CDataMap className::m_dmDataMap; \
	CDataMap *className::GetBaseMap( void ) { return NULL; } \
	CDataMap *className::GetDataMap( void ) { return &m_dmDataMap; } \
	DEFINE_DATADESC_GUTS( className )

#define DEFINE_DATADESC_GUTS( className ) \
	template <typename T> CDataMap *DataMapInit( T * ); \
	template <> CDataMap *DataMapInit<className>( className * ); \
	namespace className##_DataDescInit \
	{ \
		CDataMap *g_pDataMapHolder = DataMapInit( (className *)NULL ); \
	} \
	\
	template <> CDataMap *DataMapInit<className>( className * ) \
	{ \
		typedef className classNameTypedef; \
		className::m_dmDataMap.SetBaseMap( className::GetBaseMap() ); \

#define END_DATADESC() \
		return &classNameTypedef::m_dmDataMap; \
	}

#define GET_DATA_ADDRESS( data, offset, type )	((type *)((char *)data + offset))
#define GET_DATA( data, offset, type )			(*(GET_DATA_ADDRESS( data, offset, type )))

DEFINE_FIELDTYPE( DataField )

	DEFINE_FIELDTYPE_SAVE( pData, pFile )
	{
		return pFile->Write( GET_DATA( pData, m_uiOffset, T ) );
	}

	DEFINE_FIELDTYPE_LOAD( pData, pFile )
	{
		return pFile->Read( GET_DATA( pData, m_uiOffset, T ) );
	}

	DEFINE_FIELDTYPE_LOAD_TEXT( pData, pTextBlock )
	{
		return m_sName && pTextBlock->GetValue( GET_DATA( pData, m_uiOffset, T ), 1, m_sName );
	}

END_FIELDTYPE()

DEFINE_FIELDTYPE( EmbeddedDataField )

	DEFINE_FIELDTYPE_SAVE( pData, pFile )
	{
		return UTIL_SaveData( GET_DATA_ADDRESS( pData, m_uiOffset, T ), pFile );
	}

	DEFINE_FIELDTYPE_LOAD( pData, pFile )
	{
		return UTIL_LoadData( GET_DATA_ADDRESS( pData, m_uiOffset, T ), pFile );
	}

	DEFINE_FIELDTYPE_LOAD_TEXT( pData, pTextBlock )
	{
		if (!m_sName)
			return false;

		CTextBlock *pEmbeddedTextBlock;
		if (!pTextBlock->GetValue( pEmbeddedTextBlock, 1, m_sName ))
			return false;

		return UTIL_LoadData( GET_DATA_ADDRESS( pData, m_uiOffset, T ), pEmbeddedTextBlock );
	}

END_FIELDTYPE()

DEFINE_FIELDTYPE( VectorDataField )

	DEFINE_FIELDTYPE_SAVE( pData, pFile )
	{
		std::vector<T> &vecData = GET_DATA( pData, m_uiOffset, T );

		unsigned int uiSize = vecData.size();
		if (!pFile->Write( uiSize ))
			return false;

		for (unsigned int i = 0; i < uiSize; i++)
		{
			if (!pFile->Write( vecData[i] ))
				return false;
		}

		return true;
	}

	DEFINE_FIELDTYPE_LOAD( pData, pFile )
	{
		std::vector<T> &vecData = GET_DATA( pData, m_uiOffset, T );

		unsigned int uiSize;
		if (!pFile->Read( uiSize ))
			return false;

		for (unsigned int i = 0; i < uiSize; i++)
		{
			T tData;
			if (!pFile->Read( tData ))
				return false;

			vecData.push_back( tData );
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

		std::vector<T> &vecData = GET_DATA( pData, m_uiOffset, T );

		for (unsigned int i = 0; i < pTextLine->GetTextItemCount(); i++)
		{
			T tData;
			if (!pTextLine->GetValue( tData, i ))
				return false;

			vecData.push_back( tData );
		}

		return true;
	}

END_FIELDTYPE()

DEFINE_FIELDTYPE( EmbeddedVectorDataField )

	DEFINE_FIELDTYPE_SAVE( pData, pFile )
	{
		std::vector<T> &vecData = GET_DATA( pData, m_uiOffset, T );

		unsigned int uiSize = vecData.size();
		if (!pFile->Write( uiSize ))
			return false;

		for (unsigned int i = 0; i < uiSize; i++)
		{
			if (!UTIL_SaveData( &vecData[i], pFile ))
				return false;
		}

		return true;
	}

	DEFINE_FIELDTYPE_LOAD( pData, pFile )
	{
		std::vector<T> &vecData = GET_DATA( pData, m_uiOffset, T );

		unsigned int uiSize;
		if (!pFile->Read( uiSize ))
			return false;

		for (unsigned int i = 0; i < uiSize; i++)
		{
			T tData;
			if (!UTIL_LoadData( &tData, pFile ))
				return false;

			vecData.push_back( tData );
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

		std::vector<T> &vecData = GET_DATA( pData, m_uiOffset, T );

		for (unsigned int i = 0; i < pTextLine->GetTextItemCount(); i++)
		{
			CTextBlock *pEmbeddedTextBlock;
			if (!pTextLine->GetValue( pEmbeddedTextBlock, i ))
				return false;

			T tData;
			if (!UTIL_LoadData( &tData, pEmbeddedTextBlock ))
				return false;

			vecData.push_back( tData );
		}

		return true;
	}

END_FIELDTYPE()

#endif // DATADESC_H