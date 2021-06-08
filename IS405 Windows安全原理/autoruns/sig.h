#ifndef SIG_H
#define SIG_H

#include <windows.h>
#include <wincrypt.h>
#include <wintrust.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <softpub.h>
#include <string>
#include <QString>

//#pragma comment(lib, "crypt32.lib")
#define ENCODING (X509_ASN_ENCODING | PKCS_7_ASN_ENCODING)

typedef struct {
    LPWSTR lpszProgramName;
    LPWSTR lpszPublisherLink;
    LPWSTR lpszMoreInfoLink;
} SPROG_PUBLISHERINFO, *PSPROG_PUBLISHERINFO;

LPWSTR AllocateAndCopyWideString(LPCWSTR inputString);
int GetSignaturePublisher(TCHAR *path,QString &res, QString &timed);
BOOL GetProgAndPublisherInfo(PCMSG_SIGNER_INFO pSignerInfo,
                             PSPROG_PUBLISHERINFO Info);
BOOL PrintCertificateInfo(PCCERT_CONTEXT pCertContext,QString &res);
BOOL GetDateOfTimeStamp(PCMSG_SIGNER_INFO pSignerInfo, SYSTEMTIME* st);
//BOOL VerifyEmbeddedSignature(LPCWSTR pwszSourceFile);
BOOL GetTimeStampSignerInfo(PCMSG_SIGNER_INFO pSignerInfo,
PCMSG_SIGNER_INFO* pCounterSignerInfo);

BOOL PrintCertificateInfo(PCCERT_CONTEXT pCertContext,QString &res)
{
    BOOL fReturn = FALSE;
    LPTSTR szName = NULL;
    DWORD dwData;

    if (!(dwData = CertGetNameString(pCertContext,
                                     CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                     0,
                                     NULL,
                                     NULL,
                                     0)))
    {
        fReturn = FALSE;
        return fReturn;
    }

    // Allocate memory for subject name.
    szName = (LPTSTR)LocalAlloc(LPTR, dwData * sizeof(TCHAR));
    if (!szName)
    {
        fReturn = FALSE;
        return fReturn;
    }

    // Get subject name.
    if (!(CertGetNameString(pCertContext,
                            CERT_NAME_SIMPLE_DISPLAY_TYPE,
                            0,
                            NULL,
                            szName,
                            dwData)))
    {
        fReturn = FALSE;
        return fReturn;
    }

    // Print Subject Name.
    //*buf = QString::fromWCharArray(szName);
    fReturn = TRUE;

    //printf("%ls\n",szName);

    res = QString::fromWCharArray(szName);

    if (szName != NULL) LocalFree(szName);

    return fReturn;
}

LPWSTR AllocateAndCopyWideString(LPCWSTR inputString)
{
    LPWSTR outputString = NULL;

    outputString = (LPWSTR)LocalAlloc(LPTR,
                                      (wcslen(inputString) + 1) * sizeof(WCHAR));
    if (outputString != NULL)
    {
        lstrcpyW(outputString, inputString);
    }
    return outputString;
}

BOOL GetProgAndPublisherInfo(PCMSG_SIGNER_INFO pSignerInfo,
                             PSPROG_PUBLISHERINFO Info)
{
    BOOL fReturn = FALSE;
    PSPC_SP_OPUS_INFO OpusInfo = NULL;
    DWORD dwData;
    BOOL fResult;


    {
        // Loop through authenticated attributes and find
        // SPC_SP_OPUS_INFO_OBJID OID.
        for (DWORD n = 0; n < pSignerInfo->AuthAttrs.cAttr; n++)
        {
            if (lstrcmpA(SPC_SP_OPUS_INFO_OBJID,
                         pSignerInfo->AuthAttrs.rgAttr[n].pszObjId) == 0)
            {
                // Get Size of SPC_SP_OPUS_INFO structure.
                fResult = CryptDecodeObject(ENCODING,
                                            SPC_SP_OPUS_INFO_OBJID,
                                            pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].pbData,
                                            pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].cbData,
                                            0,
                                            NULL,
                                            &dwData);
                if (!fResult)
                {
                    _tprintf(_T("CryptDecodeObject failed with %x\n"),
                             GetLastError());
                    ;
                }

                // Allocate memory for SPC_SP_OPUS_INFO structure.
                OpusInfo = (PSPC_SP_OPUS_INFO)LocalAlloc(LPTR, dwData);
                if (!OpusInfo)
                {
                    _tprintf(_T("Unable to allocate memory for Publisher Info.\n"));
                    ;
                }

                // Decode and get SPC_SP_OPUS_INFO structure.
                fResult = CryptDecodeObject(ENCODING,
                                            SPC_SP_OPUS_INFO_OBJID,
                                            pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].pbData,
                                            pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].cbData,
                                            0,
                                            OpusInfo,
                                            &dwData);
                if (!fResult)
                {
                    _tprintf(_T("CryptDecodeObject failed with %x\n"),
                             GetLastError());
                    ;
                }

                // Fill in Program Name if present.
                if (OpusInfo->pwszProgramName)
                {
                    Info->lpszProgramName =
                            AllocateAndCopyWideString(OpusInfo->pwszProgramName);
                }
                else
                    Info->lpszProgramName = NULL;

                // Fill in Publisher Information if present.
                if (OpusInfo->pPublisherInfo)
                {

                    switch (OpusInfo->pPublisherInfo->dwLinkChoice)
                    {
                        case SPC_URL_LINK_CHOICE:
                            Info->lpszPublisherLink =
                                    AllocateAndCopyWideString(OpusInfo->pPublisherInfo->pwszUrl);
                            break;

                        case SPC_FILE_LINK_CHOICE:
                            Info->lpszPublisherLink =
                                    AllocateAndCopyWideString(OpusInfo->pPublisherInfo->pwszFile);
                            break;

                        default:
                            Info->lpszPublisherLink = NULL;
                            break;
                    }
                }
                else
                {
                    Info->lpszPublisherLink = NULL;
                }

                // Fill in More Info if present.
                if (OpusInfo->pMoreInfo)
                {
                    switch (OpusInfo->pMoreInfo->dwLinkChoice)
                    {
                        case SPC_URL_LINK_CHOICE:
                            Info->lpszMoreInfoLink =
                                    AllocateAndCopyWideString(OpusInfo->pMoreInfo->pwszUrl);
                            break;

                        case SPC_FILE_LINK_CHOICE:
                            Info->lpszMoreInfoLink =
                                    AllocateAndCopyWideString(OpusInfo->pMoreInfo->pwszFile);
                            break;

                        default:
                            Info->lpszMoreInfoLink = NULL;
                            break;
                    }
                }
                else
                {
                    Info->lpszMoreInfoLink = NULL;
                }

                fReturn = TRUE;

                break; // Break from for loop.
            } // lstrcmp SPC_SP_OPUS_INFO_OBJID
        } // for
    }

    {
        if (OpusInfo != NULL) LocalFree(OpusInfo);
    }

    return fReturn;
}

BOOL VerifyEmbeddedSignature(LPCWSTR pwszSourceFile)
{
    LONG lStatus;
    DWORD dwLastError;
    bool Success = 0;
    // Initialize the WINTRUST_FILE_INFO structure.

    WINTRUST_FILE_INFO FileData;
    memset(&FileData, 0, sizeof(FileData));
    FileData.cbStruct = sizeof(WINTRUST_FILE_INFO);
    FileData.pcwszFilePath = pwszSourceFile;
    FileData.hFile = NULL;
    FileData.pgKnownSubject = NULL;

    /*
    WVTPolicyGUID specifies the policy to apply on the file
    WINTRUST_ACTION_GENERIC_VERIFY_V2 policy checks:
    1) The certificate used to sign the file chains up to a root
    certificate located in the trusted root certificate store. This
    implies that the identity of the publisher has been verified by
    a certification authority.
    2) In cases where user interface is displayed (which this example
    does not do), WinVerifyTrust will check for whether the
    end entity certificate is stored in the trusted publisher store,
    implying that the user trusts content from this publisher.
    3) The end entity certificate has sufficient permission to sign
    code, as indicated by the presence of a code signing EKU or no
    EKU.
    */

    GUID WVTPolicyGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;
    WINTRUST_DATA WinTrustData;

    // Initialize the WinVerifyTrust input data structure.

    // Default all fields to 0.
    memset(&WinTrustData, 0, sizeof(WinTrustData));

    WinTrustData.cbStruct = sizeof(WinTrustData);

    // Use default code signing EKU.
    WinTrustData.pPolicyCallbackData = NULL;

    // No data to pass to SIP.
    WinTrustData.pSIPClientData = NULL;

    // Disable WVT UI.
    WinTrustData.dwUIChoice = WTD_UI_NONE;

    // No revocation checking.
    WinTrustData.fdwRevocationChecks = WTD_REVOKE_NONE;

    // Verify an embedded signature on a file.
    WinTrustData.dwUnionChoice = WTD_CHOICE_FILE;

    // Verify action.
    WinTrustData.dwStateAction = WTD_STATEACTION_VERIFY;

    // Verification sets this value.
    WinTrustData.hWVTStateData = NULL;

    // Not used.
    WinTrustData.pwszURLReference = NULL;

    // This is not applicable if there is no UI because it changes
    // the UI to accommodate running applications instead of
    // installing applications.
    WinTrustData.dwUIContext = 0;

    // Set pFile.
    WinTrustData.pFile = &FileData;

    // WinVerifyTrust verifies signatures as specified by the GUID
    // and Wintrust_Data.
    lStatus = WinVerifyTrust(
        NULL,
        &WVTPolicyGUID,
        &WinTrustData);

    switch (lStatus)
    {
        case ERROR_SUCCESS:
            /*
            Signed file:
                - Hash that represents the subject is trusted.
                - Trusted publisher without any verification errors.
                - UI was disabled in dwUIChoice. No publisher or
                    time stamp chain errors.
                - UI was enabled in dwUIChoice and the user clicked
                    "Yes" when asked to install and run the signed
                    subject.
            */
            Success = 1;
            // wprintf_s(L"The file \"%s\" is signed and the signature "
            //     L"was verified.\n",
            //     pwszSourceFile);
            // wprintf_s(L"The file \"%d\" is signed and the signature "
            // L"was verified.\n",
            // WVTPolicyGUID);
            break;

        case TRUST_E_NOSIGNATURE:
            // The file was not signed or had a signature
            // that was not valid.

            // Get the reason for no signature.
            dwLastError = GetLastError();
            if (TRUST_E_NOSIGNATURE == dwLastError ||
                    TRUST_E_SUBJECT_FORM_UNKNOWN == dwLastError ||
                    TRUST_E_PROVIDER_UNKNOWN == dwLastError)
            {
                // The file was not signed.
                Success = 0;
                // wprintf_s(L"The file \"%s\" is not signed.\n",
                //     pwszSourceFile);
            }
            else
            {
                // The signature was not valid or there was an error
                // opening the file.
                Success = 0;
                // wprintf_s(L"An unknown error occurred trying to "
                //     L"verify the signature of the \"%s\" file.\n",
                //     pwszSourceFile);
            }

            break;

        case TRUST_E_EXPLICIT_DISTRUST:
            // The hash that represents the subject or the publisher
            // is not allowed by the admin or user.
            Success = 0;
            // wprintf_s(L"The signature is present, but specifically "
            //     L"disallowed.\n");
            break;

        case TRUST_E_SUBJECT_NOT_TRUSTED:
            // The user clicked "No" when asked to install and run.
            Success = 0;
            // wprintf_s(L"The signature is present, but not "
            //     L"trusted.\n");
            break;

        case CRYPT_E_SECURITY_SETTINGS:
            /*
            The hash that represents the subject or the publisher
            was not explicitly trusted by the admin and the
            admin policy has disabled user trust. No signature,
            publisher or time stamp errors.
            */
            Success = 0;
            // wprintf_s(L"CRYPT_E_SECURITY_SETTINGS - The hash "
            //     L"representing the subject or the publisher wasn't "
            //     L"explicitly trusted by the admin and admin policy "
            //     L"has disabled user trust. No signature, publisher "
            //     L"or timestamp errors.\n");
            break;

        default:
            // The UI was disabled in dwUIChoice or the admin policy
            // has disabled user trust. lStatus contains the
            // publisher or time stamp chain error.
            Success = 0;
            // wprintf_s(L"Error is: 0x%x.\n",
            //     lStatus);
            break;
    }

    // Any hWVTStateData must be released by a call with close.
    WinTrustData.dwStateAction = WTD_STATEACTION_CLOSE;

    lStatus = WinVerifyTrust(
        NULL,
        &WVTPolicyGUID,
        &WinTrustData);

    return Success;
}


int GetSignaturePublisher(TCHAR *path, QString &res,QString &timed)
{
    WCHAR szFileName[MAX_PATH];
    HCERTSTORE hStore = NULL;
    HCRYPTMSG hMsg = NULL;
    PCCERT_CONTEXT pCertContext = NULL;
    BOOL fResult;
    DWORD dwEncoding, dwContentType, dwFormatType;
    PCMSG_SIGNER_INFO pSignerInfo = NULL;
    PCMSG_SIGNER_INFO pCounterSignerInfo = NULL;
    DWORD dwSignerInfo;
    CERT_INFO CertInfo;
    SPROG_PUBLISHERINFO ProgPubInfo;
    SYSTEMTIME st;

    ZeroMemory(&ProgPubInfo, sizeof(ProgPubInfo));

#ifdef UNICODE
    lstrcpynW(szFileName, path, MAX_PATH);
#endif
    // Get message handle and store handle from the signed file.
    fResult = CryptQueryObject(CERT_QUERY_OBJECT_FILE,
                               szFileName,
                               CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
                               CERT_QUERY_FORMAT_FLAG_BINARY,
                               0,
                               &dwEncoding,
                               &dwContentType,
                               &dwFormatType,
                               &hStore,
                               &hMsg,
                               NULL);
    if (!fResult)
    {
        return 1;
    }

    // Get signer information size.
    fResult = CryptMsgGetParam(hMsg,
                               CMSG_SIGNER_INFO_PARAM,
                               0,
                               NULL,
                               &dwSignerInfo);
    if (!fResult)
    {
        return 1;
    }

    // Allocate memory for signer information.
    pSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, dwSignerInfo);
    if (!pSignerInfo)
    {
        return 1;
    }

    // Get Signer Information.
    fResult = CryptMsgGetParam(hMsg,
                               CMSG_SIGNER_INFO_PARAM,
                               0,
                               (PVOID)pSignerInfo,
                               &dwSignerInfo);
    if (!fResult)
    {
        return 1;
    }

    // Get program name and publisher information from
    // signer info structure.
    GetProgAndPublisherInfo(pSignerInfo, &ProgPubInfo);

    // Search for the signer certificate in the temporary
    // certificate store.
    CertInfo.Issuer = pSignerInfo->Issuer;
    CertInfo.SerialNumber = pSignerInfo->SerialNumber;

    pCertContext = CertFindCertificateInStore(hStore,
                                              ENCODING,
                                              0,
                                              CERT_FIND_SUBJECT_CERT,
                                              (PVOID)&CertInfo,
                                              NULL);
    if (!pCertContext)
    {
        return 1;
    }

    // Print Signer certificate information.
    PrintCertificateInfo(pCertContext,res);
    if (GetTimeStampSignerInfo(pSignerInfo, &pCounterSignerInfo))
        {
            // Search for Timestamp certificate in the temporary
            // certificate store.
            CertInfo.Issuer = pCounterSignerInfo->Issuer;
            CertInfo.SerialNumber = pCounterSignerInfo->SerialNumber;

            pCertContext = CertFindCertificateInStore(hStore,
                                                      ENCODING,
                                                      0,
                                                      CERT_FIND_SUBJECT_CERT,
                                                      (PVOID) & CertInfo,
                                                      NULL);

            // Find Date of timestamp.
            //"yyyy/MM/dd hh:mm:ss"
            GetDateOfTimeStamp(pCounterSignerInfo, &st);
            TCHAR buf[200];
            TCHAR buf2[200];
            GetDateFormat(LOCALE_USER_DEFAULT,0, &st, L"yyyy/MM/dd", buf, 200);
            GetTimeFormat(LOCALE_USER_DEFAULT,0, &st, L"hh:mm:ss", buf2, 200);
            wcscat_s(buf,L" ");
            wcscat_s(buf,buf2);
            //printf("%ls\n",buf);
            timed = QString::fromWCharArray(buf);

        }
    //GetDateOfTimeStamp(pCounterSignerInfo, &st);
    //TCHAR buf[200];
    //GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &st, NULL, buf, 200);
    //printf("%ls\n",buf);


    // Clean up.
    if (ProgPubInfo.lpszProgramName != NULL)
        LocalFree(ProgPubInfo.lpszProgramName);
    if (ProgPubInfo.lpszPublisherLink != NULL)
        LocalFree(ProgPubInfo.lpszPublisherLink);
    if (ProgPubInfo.lpszMoreInfoLink != NULL)
        LocalFree(ProgPubInfo.lpszMoreInfoLink);

    if (pSignerInfo != NULL) LocalFree(pSignerInfo);
    if (pCounterSignerInfo != NULL) LocalFree(pCounterSignerInfo);
    if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);
    if (hStore != NULL) CertCloseStore(hStore, 0);
    if (hMsg != NULL) CryptMsgClose(hMsg);
    return 0;
}

BOOL GetDateOfTimeStamp(PCMSG_SIGNER_INFO pSignerInfo, SYSTEMTIME* st)
{
    BOOL fResult;
    FILETIME lft, ft;
    DWORD dwData;
    BOOL fReturn = FALSE;

    // Loop through authenticated attributes and find
    // szOID_RSA_signingTime OID.
    for (DWORD n = 0; n < pSignerInfo->AuthAttrs.cAttr; n++)
    {
        if (lstrcmpA(szOID_RSA_signingTime,
        pSignerInfo->AuthAttrs.rgAttr[n].pszObjId) == 0)
        {
            // Decode and get FILETIME structure.
            dwData = sizeof(ft);
            fResult = CryptDecodeObject(ENCODING,
            szOID_RSA_signingTime,
            pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].pbData,
            pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].cbData,
            0,
            (PVOID) & ft,
            &dwData);
            if (!fResult)
            {
                _tprintf(_T("CryptDecodeObject failed with %x\n"),
                GetLastError());
                break;
            }

            // Convert to local time.
            FileTimeToLocalFileTime(&ft, &lft);
            FileTimeToSystemTime(&lft, st);

            fReturn = TRUE;

            break; // Break from for loop.

        } //lstrcmp szOID_RSA_signingTime
    } // for

    return fReturn;
}

BOOL GetTimeStampSignerInfo(PCMSG_SIGNER_INFO pSignerInfo, PCMSG_SIGNER_INFO* pCounterSignerInfo)
{
    PCCERT_CONTEXT pCertContext = NULL;
    BOOL fReturn = FALSE;
    BOOL fResult;
    DWORD dwSize;

        *pCounterSignerInfo = NULL;

        // Loop through unathenticated attributes for
        // szOID_RSA_counterSign OID.
        for (DWORD n = 0; n < pSignerInfo->UnauthAttrs.cAttr; n++)
        {
            if (lstrcmpA(pSignerInfo->UnauthAttrs.rgAttr[n].pszObjId,
                         szOID_RSA_counterSign) == 0)
            {
                // Get size of CMSG_SIGNER_INFO structure.
                fResult = CryptDecodeObject(ENCODING,
                                            PKCS7_SIGNER_INFO,
                                            pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].pbData,
                                            pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].cbData,
                                            0,
                                            NULL,
                                            &dwSize);
                if (!fResult)
                {


                }

                // Allocate memory for CMSG_SIGNER_INFO.
                *pCounterSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, dwSize);
                if (!*pCounterSignerInfo)
                {

                }

                // Decode and get CMSG_SIGNER_INFO structure
                // for timestamp certificate.
                fResult = CryptDecodeObject(ENCODING,
                                            PKCS7_SIGNER_INFO,
                                            pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].pbData,
                                            pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].cbData,
                                            0,
                                            (PVOID) * pCounterSignerInfo,
                                            &dwSize);
                if (!fResult)
                {


                }

                fReturn = TRUE;

                break; // Break from for loop.
            }
        }


        // Clean up.
        if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);


    return fReturn;
}
#endif // SIG_H
