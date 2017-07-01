#pragma once

class IClientEntityList
{
public:
	virtual void VirtualFunction0() = 0;
	virtual void VirtualFunction1() = 0;
	virtual void VirtualFunction2() = 0;

	virtual CBaseEntity*	GetClientEntity( int iIndex ) = 0;
	virtual CBaseEntity*	GetClientEntityFromHandle( CBaseHandle hHandle ) = 0;
	virtual int				NumberOfEntities( bool bIncludeNonNetworkable ) = 0;
	virtual int				GetHighestEntityIndex() = 0;
};