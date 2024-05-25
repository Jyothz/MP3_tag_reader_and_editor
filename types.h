#ifndef TYPES_H
#define TYPES_H

typedef enum
{
	e_view,
	e_edit,
	e_help,
	e_unsupported
}OperationType;

/*used in function return type*/
typedef enum
{
	e_success,
	e_failure
}Status;
#endif
