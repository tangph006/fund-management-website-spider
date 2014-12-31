#pragma once

enum CTPDemoErrorCode 
{
    Success0,
    WarnBegin,
    WarnSameLoggerExists,
    WarnEnd,
    ErrorBegin,
    ErrorInvalidFilePath,
    ErrorOpenFileFailed,
    ErrorCreateFileMapFailed,
    ErrorMapViewOfFileFailed,
    ErrorDiffLengthLoggerExists,
    ErrorEnd
};