
/*
    Brew.js API - C++ core
    Switch API -> Module -> nx
*/

#include <switch.h>
#include "../duktape/duktape.h"

namespace nx
{
    inline int savesIDList(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int idx = duk_get_number(ctx, 0);
            vector<u64> ids;
            FsSaveDataIterator iterator;
            FsSaveDataInfo info;
            size_t total_entries = 0;
            size_t outsize = 0;
            NsApplicationControlData* nsacd = (NsApplicationControlData*)malloc(sizeof(NsApplicationControlData));
            memset(nsacd, 0, sizeof(NsApplicationControlData));
            if(R_SUCCEEDED(fsOpenSaveDataIterator(&iterator, FsSaveDataSpaceId_NandUser)))
            {
                while(1)
                {
                    fsSaveDataIteratorRead(&iterator, &info, 1, &total_entries);
                    if(total_entries == 0) break;
                    if(info.SaveDataType == FsSaveDataType_SaveData)
                    {
                        u64 tid = info.titleID;
                        ids.push_back(tid);
                    }
                }
                free(nsacd);
                fsSaveDataIteratorClose(&iterator);
                u64 out = ids.at(idx);
                duk_push_number(ctx, out);
            }
        }
        return 1;
    }

    inline int savesIDList_count(duk_context *ctx)
    {
        int count = 0;
        FsSaveDataIterator iterator;
        FsSaveDataInfo info;
        size_t total_entries = 0;
        size_t outsize = 0;
        NsApplicationControlData* nsacd = (NsApplicationControlData*)malloc(sizeof(NsApplicationControlData));
        memset(nsacd, 0, sizeof(NsApplicationControlData));
        if(R_SUCCEEDED(fsOpenSaveDataIterator(&iterator, FsSaveDataSpaceId_NandUser)))
        {
            while(1)
            {
                fsSaveDataIteratorRead(&iterator, &info, 1, &total_entries);
                if(total_entries == 0) break;
                if(info.SaveDataType == FsSaveDataType_SaveData)
                {
                    count++;
                }
            }
            free(nsacd);
            fsSaveDataIteratorClose(&iterator);
            duk_push_number(ctx, count);
        }
        return 1;
    }

    inline int Title_nacp_entry_name(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 2)
        {
            u64 id = duk_get_number(ctx, 0);
            int idx = duk_get_number(ctx, 1);
            NsApplicationControlData *nsapp = (NsApplicationControlData*)malloc(sizeof(NsApplicationControlData));;
            memset(nsapp, 0, sizeof(NsApplicationControlData));
            NacpLanguageEntry *langentry;
            size_t outsize = 0;
            if(R_SUCCEEDED(nsInitialize()))
            {
                if(R_SUCCEEDED(nsGetApplicationControlData(1, id, nsapp, sizeof(NsApplicationControlData), &outsize)))
                {
                    if(R_SUCCEEDED(nacpGetLanguageEntry(&nsapp->nacp, &langentry)))
                    {
                        NacpLanguageEntry lent = nsapp->nacp.lang[idx];
                        duk_push_string(ctx, lent.name);
                    }
                }
                nsExit();
            }
        }
        return 1;
    }

    inline int Title_nacp_entry_author(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 2)
        {
            u64 id = duk_get_number(ctx, 0);
            int idx = duk_get_number(ctx, 1);
            NsApplicationControlData *nsapp = (NsApplicationControlData*)malloc(sizeof(NsApplicationControlData));;
            memset(nsapp, 0, sizeof(NsApplicationControlData));
            NacpLanguageEntry *langentry;
            size_t outsize = 0;
            if(R_SUCCEEDED(nsInitialize()))
            {
                if(R_SUCCEEDED(nsGetApplicationControlData(1, id, nsapp, sizeof(NsApplicationControlData), &outsize)))
                {
                    if(R_SUCCEEDED(nacpGetLanguageEntry(&nsapp->nacp, &langentry)))
                    {
                        NacpLanguageEntry lent = nsapp->nacp.lang[idx];
                        duk_push_string(ctx, lent.author);
                    }
                }
                nsExit();
            }
        }
        return 1;
    }

    inline int Title_nacp_version(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            u64 id = duk_get_number(ctx, 0);
            NsApplicationControlData *nsapp = (NsApplicationControlData*)malloc(sizeof(NsApplicationControlData));;
            memset(nsapp, 0, sizeof(NsApplicationControlData));
            size_t outsize = 0;
            if(R_SUCCEEDED(nsInitialize()))
            {
                if(R_SUCCEEDED(nsGetApplicationControlData(1, id, nsapp, sizeof(NsApplicationControlData), &outsize)))
                {
                    duk_push_string(ctx, nsapp->nacp.version);
                }
                nsExit();
            }
        }
        return 1;
    }

    inline int Title_icon(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 2)
        {
            u64 id = duk_get_number(ctx, 0);
            const char *out = duk_get_string(ctx, 1);
            NsApplicationControlData *nsapp = (NsApplicationControlData*)malloc(sizeof(NsApplicationControlData));;
            memset(nsapp, 0, sizeof(NsApplicationControlData));
            size_t outsize = 0;
            if(R_SUCCEEDED(nsInitialize()))
            {
                if(R_SUCCEEDED(nsGetApplicationControlData(1, id, nsapp, sizeof(NsApplicationControlData), &outsize)))
                {
                    FILE *ficon = fopen(out, "wb");
                    if(ficon)
                    {
                        fwrite(nsapp->icon, sizeof(u8), sizeof(nsapp->icon), ficon);
                    }
                    fclose(ficon);
                }
                nsExit();
            }
        }
        return 1;
    }

    inline int language(duk_context *ctx)
    {
        if(R_SUCCEEDED(setInitialize()))
        {
            u64 lang;
            if(R_SUCCEEDED(setGetSystemLanguage(&lang)))
            {
                string slang = string((const char*)&lang);
                string rlang;
                if(slang == "ja") rlang = "Japanese";
                else if(slang == "en-US") rlang = "American English";
                else if(slang == "fr") rlang = "French";
                else if(slang == "de") rlang = "German";
                else if(slang == "it") rlang = "Italian";
                else if(slang == "es") rlang = "Spanish";
                else if(slang == "zh-CN") rlang = "Chinese";
                else if(slang == "ko") rlang = "Korean";
                else if(slang == "nl") rlang = "Dutch";
                else if(slang == "pt") rlang = "Portuguese";
                else if(slang == "ru") rlang = "Russian";
                else if(slang == "zh-TW") rlang = "Taiwanese";
                else if(slang == "en-GB") rlang = "British English";
                else if(slang == "fr-CA") rlang = "Canadian French";
                else if(slang == "es-419") rlang = "Latin American Spanish";
                else if(slang == "zh-Hans") rlang = "Simplified Chinese";
                else if(slang == "zh-Hant") rlang = "Traditional Chinese";
                else rlang = "Unknown";
                duk_push_string(ctx, rlang.c_str());
            }
        }
        return 1;
    }

    void Context(duk_context *ctx)
    {
        duk_push_c_function(ctx, Title_nacp_version, DUK_VARARGS);
	    duk_put_global_string(ctx, "__nx__Title_nacp_version");
        duk_push_c_function(ctx, Title_nacp_entry_name, DUK_VARARGS);
	    duk_put_global_string(ctx, "__nx__Title_nacp_entry_name");
        duk_push_c_function(ctx, Title_nacp_entry_author, DUK_VARARGS);
	    duk_put_global_string(ctx, "__nx__Title_nacp_entry_author");
        duk_push_c_function(ctx, Title_icon, DUK_VARARGS);
	    duk_put_global_string(ctx, "__nx__Title_icon");
        duk_push_c_function(ctx, savesIDList, DUK_VARARGS);
	    duk_put_global_string(ctx, "__nx__savesIDList");
        duk_push_c_function(ctx, savesIDList_count, DUK_VARARGS);
	    duk_put_global_string(ctx, "__nx__savesIDList_count");
        duk_push_c_function(ctx, language, DUK_VARARGS);
	    duk_put_global_string(ctx, "__nx__language");
    }
}