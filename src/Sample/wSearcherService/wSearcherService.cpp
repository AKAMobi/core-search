// Beeper Service.cpp

// Copyright (c) 2001-2002 Jarmo Muukka. All rights reserved.

#include "wSearcher.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <direct.h>

#define MAX_DIR_LEN 1024

TCHAR* serviceName = TEXT("coreSearch");
SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE serviceStatusHandle = 0;
HANDLE stopServiceEvent = 0;

void WINAPI ServiceControlHandler( DWORD controlCode )
{
	switch ( controlCode )
	{
		case SERVICE_CONTROL_INTERROGATE:
			break;

		case SERVICE_CONTROL_SHUTDOWN:
		case SERVICE_CONTROL_STOP:
			serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
			SetServiceStatus( serviceStatusHandle, &serviceStatus );

			SetEvent( stopServiceEvent );
			return;

		case SERVICE_CONTROL_PAUSE:
			break;

		case SERVICE_CONTROL_CONTINUE:
			break;

		default:
			if ( controlCode >= 128 && controlCode <= 255 )
				// user defined control code
				break;
			else
				// unrecognised control code
				break;
	}

	SetServiceStatus( serviceStatusHandle, &serviceStatus );
}

void WINAPI ServiceMain( DWORD argc, TCHAR* argv[] )
{
	// initialise service status
	serviceStatus.dwServiceType = SERVICE_WIN32;
	serviceStatus.dwCurrentState = SERVICE_STOPPED;
	serviceStatus.dwControlsAccepted = 0;
	serviceStatus.dwWin32ExitCode = NO_ERROR;
	serviceStatus.dwServiceSpecificExitCode = NO_ERROR;
	serviceStatus.dwCheckPoint = 0;
	serviceStatus.dwWaitHint = 0;

	serviceStatusHandle = RegisterServiceCtrlHandler( serviceName, ServiceControlHandler );

	if ( serviceStatusHandle )
	{
		char buffer[MAX_DIR_LEN];
		// service is starting
		serviceStatus.dwCurrentState = SERVICE_START_PENDING;
		SetServiceStatus( serviceStatusHandle, &serviceStatus );

		// do initialisation here
		stopServiceEvent = CreateEvent( 0, FALSE, FALSE, 0 );

		// running
		serviceStatus.dwControlsAccepted |= (SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);
		serviceStatus.dwCurrentState = SERVICE_RUNNING;
		SetServiceStatus( serviceStatusHandle, &serviceStatus );

		//if (init_search("C:\\Program Files\\AKA-coreSearch\\",0)!=-1) 
		{
			HMODULE module;
			int i;
			module = GetModuleHandle(0);
			GetModuleFileName(module, buffer, MAX_DIR_LEN);
			for ( i=strlen(buffer); i>0; i-- ){
				if ( buffer[i]=='\\' ){
					buffer[i+1]=0;
					break;
				}
			}
		}
/*		{
			FILE* fp=fopen("c:\\coreSearch.log","a");
			fprintf(fp,"init dir [%s]\n", buffer);
			fclose(fp);
		}
		exit(0);
*/
		if (init_search(buffer,0)!=-1) 
		{
			struct sockaddr_in sin;
			int   numfds, i;
			struct timeval to;
			int selectErrors = 0;
			int maxfd = 0, usert = 0;
			fd_set readfds;
			fd_set bakfds;
			SOCKET sockfd, csock;
			int value;

			WSADATA wsaData;
			WSAStartup(MAKEWORD(2,2),&wsaData);
			memset(&sin, 0, sizeof(sin));
			sin.sin_family = AF_INET;
			sin.sin_addr.s_addr = htonl(INADDR_ANY);
			sin.sin_port = htons(LISTEN_PORT);
			sockfd = socket(AF_INET, SOCK_STREAM, 0);
			set_nonblocking(sockfd);
			if (!((bind(sockfd, (struct sockaddr *) &sin, sizeof(sin)) < 0) || (listen(sockfd, 5) < 0))) {
			// by zixia qfp = fopen("query.log", "a");	//保存查询内容

				FD_ZERO(&bakfds);
				FD_SET (sockfd, &bakfds);
				maxfd=sockfd;

				do
				{
					to.tv_usec = 0;
					to.tv_sec = 1;
					readfds = bakfds;
					numfds = select (maxfd + 1, &readfds, 0, 0, &to);
					if (numfds == -1)
					{
						if (++selectErrors == 5)
						{
							break;
						}
						continue;
					}
					if(numfds==0)
						continue;
					selectErrors = 0;
					if (FD_ISSET (sockfd, &readfds))
					{
						int sinsize=sizeof(sin);
						SOCKET f;
						if ((f = accept (sockfd, (struct sockaddr *) &sin, &sinsize)) > 0)
						{
							if(f<CONNECTION_NUM)
							{
								g_tcp[usert].fd=f;
								g_tcp[usert].ip=sin.sin_addr.s_addr;
								g_tcp[usert].port=sin.sin_port;
								g_tcp[usert].buft=0;
								usert ++;
								set_nonblocking(f);
								FD_SET (f,&bakfds);
							}
							else
							{
								closesocket(f);
							}
						}
					}
					for(i=0;i<usert;i++)
					{
						if(g_tcp[i].fd && FD_ISSET(g_tcp[i].fd,&readfds))
						{
							if(handle_connection(i)<=0)
							{
								FD_CLR(g_tcp[i].fd,&bakfds);
								closesocket(g_tcp[i].fd);
								memcpy(&g_tcp[i],&g_tcp[usert-1],sizeof(TcpInfo));
								usert --;
							}
						}    
					}
		// by zixia fclose(qfp);
				}
				while ( WaitForSingleObject( stopServiceEvent, 0 ) == WAIT_TIMEOUT );
			}
			// 只有init_search成功，最后才done_search, by zixia 2004-05-19
			done_search();
			WSACleanup();

		}
		else {
			FILE* fp=fopen("c:\\coreSearch.log","a");
			fprintf(fp,"error: cannot init lib\n");
			fclose(fp);
		}
	    
		// service was stopped
		serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus( serviceStatusHandle, &serviceStatus );

		// do cleanup here
		CloseHandle( stopServiceEvent );
		stopServiceEvent = 0;

		// service is now stopped
		serviceStatus.dwControlsAccepted &= ~(SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);
		serviceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus( serviceStatusHandle, &serviceStatus );
	}
}

void RunService()
{
	SERVICE_TABLE_ENTRY serviceTable[] =
	{
		{ serviceName, ServiceMain },
		{ 0, 0 }
	};

	StartServiceCtrlDispatcher( serviceTable );
}

void InstallService()
{
	SC_HANDLE serviceControlManager = OpenSCManager( 0, 0, SC_MANAGER_CREATE_SERVICE );

	if ( serviceControlManager )
	{
		TCHAR path[ _MAX_PATH + 1 ];
		if ( GetModuleFileName( 0, path, sizeof(path)/sizeof(path[0]) ) > 0 )
		{
			SC_HANDLE service = CreateService( serviceControlManager,
							serviceName, serviceName,
							SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
							SERVICE_AUTO_START, SERVICE_ERROR_IGNORE, path,
							0, 0, 0, 0, 0 );
			if ( service ){
				SERVICE_DESCRIPTION Info;
				Info.lpDescription = "AKA coreSearch全文检索引擎";
				ChangeServiceConfig2 ( service, SERVICE_CONFIG_DESCRIPTION, &Info );
				CloseServiceHandle( service );
			}
		}

		CloseServiceHandle( serviceControlManager );
	}
}

void UninstallService()
{
	SC_HANDLE serviceControlManager = OpenSCManager( 0, 0, SC_MANAGER_CONNECT );

	if ( serviceControlManager )
	{
		SC_HANDLE service = OpenService( serviceControlManager,
			serviceName, SERVICE_QUERY_STATUS | DELETE );
		if ( service )
		{
			SERVICE_STATUS serviceStatus;
			if ( QueryServiceStatus( service, &serviceStatus ) )
			{
				if ( serviceStatus.dwCurrentState == SERVICE_STOPPED )
					DeleteService( service );
			}

			CloseServiceHandle( service );
		}

		CloseServiceHandle( serviceControlManager );
	}
}

int _tmain( int argc, TCHAR* argv[] )
{
	if ( argc > 1 && lstrcmpi( argv[1], TEXT("install") ) == 0 )
	{
		InstallService();
	}
	else if ( argc > 1 && lstrcmpi( argv[1], TEXT("uninstall") ) == 0 )
	{
		UninstallService();
	}
	else
	{
		RunService();
	}

	return 0;
}


