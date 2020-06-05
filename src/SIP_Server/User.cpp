#include "SIP_Server.h"

int CSIP_Server::Set_User_Accont()
{

	// Reload CDMS Data
	CDMS_DATA Cdms_Data;

	// Set 1st user data
	memset(&Cdms_Data, 0, sizeof(Cdms_Data));
	Cdms_Data.Active = TRUE;
	strcpy(Cdms_Data.PhoneNo, "0987654321");
	strcpy(Cdms_Data.ID, "001010123456100");	// IMSI for reg
	strcpy(Cdms_Data.Name, "0987654321");
	strcpy(Cdms_Data.Password, "1234");
	strcpy(Cdms_Data.Remarks, "SIP");
	if (Check_CDMS_Data(Cdms_Data.PhoneNo) < 0)
		Save_CDMS_Data(&Cdms_Data);

	// Set 2nd user data
	memset(&Cdms_Data, 0, sizeof(Cdms_Data));
	Cdms_Data.Active = TRUE;
	strcpy(Cdms_Data.PhoneNo, "0987654322");
	strcpy(Cdms_Data.ID, "001010123456075");
	strcpy(Cdms_Data.Name, "0987654322");
	strcpy(Cdms_Data.Password, "1234");
	strcpy(Cdms_Data.Remarks, "SIP");
	if (Check_CDMS_Data(Cdms_Data.PhoneNo) < 0)
		Save_CDMS_Data(&Cdms_Data);

	// Set 3rt user data
	memset(&Cdms_Data, 0, sizeof(Cdms_Data));
	Cdms_Data.Active = TRUE;
	strcpy(Cdms_Data.PhoneNo, "0987654323");
	strcpy(Cdms_Data.ID, "001010123456063");
	strcpy(Cdms_Data.Name, "0987654322");
	strcpy(Cdms_Data.Password, "1234");
	strcpy(Cdms_Data.Remarks, "SIP");
	if (Check_CDMS_Data(Cdms_Data.PhoneNo) < 0)
		Save_CDMS_Data(&Cdms_Data);

	// Set 4th user data
	memset(&Cdms_Data, 0, sizeof(Cdms_Data));
	Cdms_Data.Active = TRUE;
	strcpy(Cdms_Data.PhoneNo, "0987654331");
	strcpy(Cdms_Data.ID, "0987654331");
	strcpy(Cdms_Data.Name, "0987654331");
	strcpy(Cdms_Data.Password, "1234");
	strcpy(Cdms_Data.Remarks, "SIP");
	if (Check_CDMS_Data(Cdms_Data.PhoneNo) < 0)
		Save_CDMS_Data(&Cdms_Data);

	// Set tth user data
	memset(&Cdms_Data, 0, sizeof(Cdms_Data));
	Cdms_Data.Active = TRUE;
	strcpy(Cdms_Data.PhoneNo, "0987654332");
	strcpy(Cdms_Data.ID, "0987654332");
	strcpy(Cdms_Data.Name, "0987654332");
	strcpy(Cdms_Data.Password, "1234");
	strcpy(Cdms_Data.Remarks, "SIP");
	if (Check_CDMS_Data(Cdms_Data.PhoneNo) < 0)
		Save_CDMS_Data(&Cdms_Data);

	return 0;
}
