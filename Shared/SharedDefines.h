#ifndef SHAREDDEFINES_H
#define SHAREDDEFINES_H

#define DECLARE_CLASS( x, y )			typedef x ThisClass; \
										typedef y BaseClass;

#define DECLARE_CLASS_NOBASE( x )		typedef x ThisClass;

#endif // SHAREDDEFINES_H