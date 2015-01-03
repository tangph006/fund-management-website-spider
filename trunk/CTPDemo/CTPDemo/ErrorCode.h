#pragma once

enum CTPDemoErrorCode 
{
    Success0,
    WarnBegin,
    WarnSameLoggerExists,
    WarnInstrumentTypeExists,
    WarnEnd,
    ErrorBegin,
    ErrorInvalidFilePath,
    ErrorOpenFileFailed,
    ErrorCreateFileMapFailed,
    ErrorMapViewOfFileFailed,
    ErrorDiffLengthLoggerExists,
    ErrorEnd
};