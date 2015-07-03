//
// MATLAB Compiler: 4.13 (R2010a)
// Date: Fri Apr 09 23:52:52 2010
// Arguments: "-B" "macro_default" "-W" "cpplib:AnalizePulses" "-T" "link:lib"
// "AnalizarPulsos" 
//

#include <stdio.h>
#define EXPORTING_AnalizePulses 1
#include "AnalizePulses.h"
#ifdef __cplusplus
extern "C" {
#endif

extern mclComponentData __MCC_AnalizePulses_component_data;

#ifdef __cplusplus
}
#endif


static HMCRINSTANCE _mcr_inst = NULL;


#if defined( _MSC_VER) || defined(__BORLANDC__) || defined(__WATCOMC__) || defined(__LCC__)
#ifdef __LCC__
#undef EXTERN_C
#endif
#include <windows.h>

static wchar_t path_to_dll[_MAX_PATH];

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, void *pv)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        if (GetModuleFileName(hInstance, path_to_dll, _MAX_PATH) == 0)
            return FALSE;
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
    }
    return TRUE;
}
#endif
#ifdef __cplusplus
extern "C" {
#endif

static int mclDefaultPrintHandler(const char *s)
{
  return mclWrite(1 /* stdout */, s, sizeof(char)*strlen(s));
}

#ifdef __cplusplus
} /* End extern "C" block */
#endif

#ifdef __cplusplus
extern "C" {
#endif

static int mclDefaultErrorHandler(const char *s)
{
  int written = 0;
  size_t len = 0;
  len = strlen(s);
  written = mclWrite(2 /* stderr */, s, sizeof(char)*len);
  if (len > 0 && s[ len-1 ] != '\n')
    written += mclWrite(2 /* stderr */, "\n", sizeof(char));
  return written;
}

#ifdef __cplusplus
} /* End extern "C" block */
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_AnalizePulses_C_API
#define LIB_AnalizePulses_C_API /* No special import/export declaration */
#endif

LIB_AnalizePulses_C_API 
bool MW_CALL_CONV AnalizePulsesInitializeWithHandlers(
    mclOutputHandlerFcn error_handler,
    mclOutputHandlerFcn print_handler)
{
    int bResult = 0;
  if (_mcr_inst != NULL)
    return true;
  if (!mclmcrInitialize())
    return false;
  if (!GetModuleFileName(GetModuleHandle(L"AnalizePulses"), path_to_dll, _MAX_PATH))
    return false;
    {
        mclCtfStream ctfStream = 
            mclGetEmbeddedCtfStream(path_to_dll, 
                                    50170);
        if (ctfStream) {
            bResult = mclInitializeComponentInstanceEmbedded(   &_mcr_inst,
                                                                
                                                     &__MCC_AnalizePulses_component_data,
                                                                true, 
                                                                NoObjectType, 
                                                                LibTarget,
                                                                error_handler, 
                                                                print_handler,
                                                                ctfStream, 
                                                                50170);
            mclDestroyStream(ctfStream);
        } else {
            bResult = 0;
        }
    }  
    if (!bResult)
    return false;
  return true;
}

LIB_AnalizePulses_C_API 
bool MW_CALL_CONV AnalizePulsesInitialize(void)
{
  return AnalizePulsesInitializeWithHandlers(mclDefaultErrorHandler, 
                                             mclDefaultPrintHandler);
}

LIB_AnalizePulses_C_API 
void MW_CALL_CONV AnalizePulsesTerminate(void)
{
  if (_mcr_inst != NULL)
    mclTerminateInstance(&_mcr_inst);
}

LIB_AnalizePulses_C_API 
long MW_CALL_CONV AnalizePulsesGetMcrID() 
{
  return mclGetID(_mcr_inst);
}

LIB_AnalizePulses_C_API 
void MW_CALL_CONV AnalizePulsesPrintStackTrace(void) 
{
  char** stackTrace;
  int stackDepth = mclGetStackTrace(_mcr_inst, &stackTrace);
  int i;
  for(i=0; i<stackDepth; i++)
  {
    mclWrite(2 /* stderr */, stackTrace[i], sizeof(char)*strlen(stackTrace[i]));
    mclWrite(2 /* stderr */, "\n", sizeof(char)*strlen("\n"));
  }
  mclFreeStackTrace(&stackTrace, stackDepth);
}


LIB_AnalizePulses_C_API 
bool MW_CALL_CONV mlxAnalizarPulsos(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "AnalizarPulsos", nlhs, plhs, nrhs, prhs);
}

LIB_AnalizePulses_CPP_API 
void MW_CALL_CONV AnalizarPulsos(int nargout, mwArray& sequencia, const mwArray& archivo, 
                                 const mwArray& ploted)
{
  mclcppMlfFeval(_mcr_inst, "AnalizarPulsos", nargout, 1, 2, &sequencia, &archivo, &ploted);
}
