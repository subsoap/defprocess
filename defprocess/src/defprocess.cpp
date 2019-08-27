// Extension lib defines
#define LIB_NAME "defprocess"
#define MODULE_NAME "defprocess"

// include the Defold SDK
#include <dmsdk/sdk.h>
#include <stdlib.h>

#if defined(DM_PLATFORM_WINDOWS) 
#include <windows.h>
#include <tlhelp32.h>

bool GetProcessHandle(const char *process_name, DWORD dwAccess)
{
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32 = {0};

    hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

    if(hProcessSnap==INVALID_HANDLE_VALUE)
    {
        CloseHandle( hProcessSnap );
        return FALSE;
    }

    pe32.dwSize=sizeof(PROCESSENTRY32);

    if(!Process32First(hProcessSnap,&pe32))
    {
        CloseHandle( hProcessSnap );
        return FALSE;
    }

    do
    {
        if(strcmp(pe32.szExeFile,process_name)==0)
        {
            CloseHandle( hProcessSnap );
            return TRUE;
        }
    }
    while(Process32Next(hProcessSnap,&pe32));

    CloseHandle( hProcessSnap );
    return FALSE;
}

static int is_process_running(lua_State *L)
{
    const char *process_name = luaL_checkstring(L, 1);

    lua_pushboolean(L, GetProcessHandle(process_name,PROCESS_QUERY_INFORMATION) == TRUE);

    return 1;
}


// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
    {"is_process_running", is_process_running},
    {0, 0}
};

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);

    // Register lua names
    luaL_register(L, MODULE_NAME, Module_methods);

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

dmExtension::Result AppInitializeMyExtension(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeMyExtension(dmExtension::Params* params)
{
    // Init Lua
    LuaInit(params->m_L);
    printf("Registered %s Extension\n", MODULE_NAME);
    return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeMyExtension(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeMyExtension(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}


//////////////////////

DM_DECLARE_EXTENSION(defprocess, LIB_NAME, AppInitializeMyExtension, AppFinalizeMyExtension, InitializeMyExtension, 0, 0, FinalizeMyExtension)
#else


dmExtension::Result InitializeMyExtension(dmExtension::Params *params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeMyExtension(dmExtension::Params *params)
{
    return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(defprocess, LIB_NAME, 0, 0, InitializeMyExtension, 0, 0, FinalizeMyExtension)
#endif
