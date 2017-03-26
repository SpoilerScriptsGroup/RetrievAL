.486
.model flat, c

extrn _imp_NONAME0                     :dword
extrn _imp_CloseDriver                 :dword
extrn _imp_DefDriverProc               :dword
extrn _imp_DriverCallback              :dword
extrn _imp_DrvGetModuleHandle          :dword
extrn _imp_GetDriverModuleHandle       :dword
extrn _imp_MigrateAllDrivers           :dword
extrn _imp_MigrateMidiUser             :dword
extrn _imp_MigrateSoundEvents          :dword
extrn _imp_NotifyCallbackData          :dword
extrn _imp_OpenDriver                  :dword
extrn _imp_PlaySound                   :dword
extrn _imp_PlaySoundA                  :dword
extrn _imp_PlaySoundW                  :dword
extrn _imp_SendDriverMessage           :dword
extrn _imp_WOW32DriverCallback         :dword
extrn _imp_WOW32ResolveMultiMediaHandle:dword
extrn _imp_WOWAppExit                  :dword
extrn _imp_WinmmLogoff                 :dword
extrn _imp_WinmmLogon                  :dword
extrn _imp_aux32Message                :dword
extrn _imp_auxGetDevCapsA              :dword
extrn _imp_auxGetDevCapsW              :dword
extrn _imp_auxGetNumDevs               :dword
extrn _imp_auxGetVolume                :dword
extrn _imp_auxOutMessage               :dword
extrn _imp_auxSetVolume                :dword
extrn _imp_joy32Message                :dword
extrn _imp_joyConfigChanged            :dword
extrn _imp_joyGetDevCapsA              :dword
extrn _imp_joyGetDevCapsW              :dword
extrn _imp_joyGetNumDevs               :dword
extrn _imp_joyGetPos                   :dword
extrn _imp_joyGetPosEx                 :dword
extrn _imp_joyGetThreshold             :dword
extrn _imp_joyReleaseCapture           :dword
extrn _imp_joySetCapture               :dword
extrn _imp_joySetThreshold             :dword
extrn _imp_mci32Message                :dword
extrn _imp_mciDriverNotify             :dword
extrn _imp_mciDriverYield              :dword
extrn _imp_mciExecute                  :dword
extrn _imp_mciFreeCommandResource      :dword
extrn _imp_mciGetCreatorTask           :dword
extrn _imp_mciGetDeviceIDA             :dword
extrn _imp_mciGetDeviceIDFromElementIDA:dword
extrn _imp_mciGetDeviceIDFromElementIDW:dword
extrn _imp_mciGetDeviceIDW             :dword
extrn _imp_mciGetDriverData            :dword
extrn _imp_mciGetErrorStringA          :dword
extrn _imp_mciGetErrorStringW          :dword
extrn _imp_mciGetYieldProc             :dword
extrn _imp_mciLoadCommandResource      :dword
extrn _imp_mciSendCommandA             :dword
extrn _imp_mciSendCommandW             :dword
extrn _imp_mciSendStringA              :dword
extrn _imp_mciSendStringW              :dword
extrn _imp_mciSetDriverData            :dword
extrn _imp_mciSetYieldProc             :dword
extrn _imp_mid32Message                :dword
extrn _imp_midiConnect                 :dword
extrn _imp_midiDisconnect              :dword
extrn _imp_midiInAddBuffer             :dword
extrn _imp_midiInClose                 :dword
extrn _imp_midiInGetDevCapsA           :dword
extrn _imp_midiInGetDevCapsW           :dword
extrn _imp_midiInGetErrorTextA         :dword
extrn _imp_midiInGetErrorTextW         :dword
extrn _imp_midiInGetID                 :dword
extrn _imp_midiInGetNumDevs            :dword
extrn _imp_midiInMessage               :dword
extrn _imp_midiInOpen                  :dword
extrn _imp_midiInPrepareHeader         :dword
extrn _imp_midiInReset                 :dword
extrn _imp_midiInStart                 :dword
extrn _imp_midiInStop                  :dword
extrn _imp_midiInUnprepareHeader       :dword
extrn _imp_midiOutCacheDrumPatches     :dword
extrn _imp_midiOutCachePatches         :dword
extrn _imp_midiOutClose                :dword
extrn _imp_midiOutGetDevCapsA          :dword
extrn _imp_midiOutGetDevCapsW          :dword
extrn _imp_midiOutGetErrorTextA        :dword
extrn _imp_midiOutGetErrorTextW        :dword
extrn _imp_midiOutGetID                :dword
extrn _imp_midiOutGetNumDevs           :dword
extrn _imp_midiOutGetVolume            :dword
extrn _imp_midiOutLongMsg              :dword
extrn _imp_midiOutMessage              :dword
extrn _imp_midiOutOpen                 :dword
extrn _imp_midiOutPrepareHeader        :dword
extrn _imp_midiOutReset                :dword
extrn _imp_midiOutSetVolume            :dword
extrn _imp_midiOutShortMsg             :dword
extrn _imp_midiOutUnprepareHeader      :dword
extrn _imp_midiStreamClose             :dword
extrn _imp_midiStreamOpen              :dword
extrn _imp_midiStreamOut               :dword
extrn _imp_midiStreamPause             :dword
extrn _imp_midiStreamPosition          :dword
extrn _imp_midiStreamProperty          :dword
extrn _imp_midiStreamRestart           :dword
extrn _imp_midiStreamStop              :dword
extrn _imp_mixerClose                  :dword
extrn _imp_mixerGetControlDetailsA     :dword
extrn _imp_mixerGetControlDetailsW     :dword
extrn _imp_mixerGetDevCapsA            :dword
extrn _imp_mixerGetDevCapsW            :dword
extrn _imp_mixerGetID                  :dword
extrn _imp_mixerGetLineControlsA       :dword
extrn _imp_mixerGetLineControlsW       :dword
extrn _imp_mixerGetLineInfoA           :dword
extrn _imp_mixerGetLineInfoW           :dword
extrn _imp_mixerGetNumDevs             :dword
extrn _imp_mixerMessage                :dword
extrn _imp_mixerOpen                   :dword
extrn _imp_mixerSetControlDetails      :dword
extrn _imp_mmDrvInstall                :dword
extrn _imp_mmGetCurrentTask            :dword
extrn _imp_mmTaskBlock                 :dword
extrn _imp_mmTaskCreate                :dword
extrn _imp_mmTaskSignal                :dword
extrn _imp_mmTaskYield                 :dword
extrn _imp_mmioAdvance                 :dword
extrn _imp_mmioAscend                  :dword
extrn _imp_mmioClose                   :dword
extrn _imp_mmioCreateChunk             :dword
extrn _imp_mmioDescend                 :dword
extrn _imp_mmioFlush                   :dword
extrn _imp_mmioGetInfo                 :dword
extrn _imp_mmioInstallIOProcA          :dword
extrn _imp_mmioInstallIOProcW          :dword
extrn _imp_mmioOpenA                   :dword
extrn _imp_mmioOpenW                   :dword
extrn _imp_mmioRead                    :dword
extrn _imp_mmioRenameA                 :dword
extrn _imp_mmioRenameW                 :dword
extrn _imp_mmioSeek                    :dword
extrn _imp_mmioSendMessage             :dword
extrn _imp_mmioSetBuffer               :dword
extrn _imp_mmioSetInfo                 :dword
extrn _imp_mmioStringToFOURCCA         :dword
extrn _imp_mmioStringToFOURCCW         :dword
extrn _imp_mmioWrite                   :dword
extrn _imp_mmsystemGetVersion          :dword
extrn _imp_mod32Message                :dword
extrn _imp_mxd32Message                :dword
extrn _imp_sndPlaySoundA               :dword
extrn _imp_sndPlaySoundW               :dword
extrn _imp_tid32Message                :dword
extrn _imp_timeBeginPeriod             :dword
extrn _imp_timeEndPeriod               :dword
extrn _imp_timeGetDevCaps              :dword
extrn _imp_timeGetSystemTime           :dword
extrn _imp_timeGetTime                 :dword
extrn _imp_timeKillEvent               :dword
extrn _imp_timeSetEvent                :dword
extrn _imp_waveInAddBuffer             :dword
extrn _imp_waveInClose                 :dword
extrn _imp_waveInGetDevCapsA           :dword
extrn _imp_waveInGetDevCapsW           :dword
extrn _imp_waveInGetErrorTextA         :dword
extrn _imp_waveInGetErrorTextW         :dword
extrn _imp_waveInGetID                 :dword
extrn _imp_waveInGetNumDevs            :dword
extrn _imp_waveInGetPosition           :dword
extrn _imp_waveInMessage               :dword
extrn _imp_waveInOpen                  :dword
extrn _imp_waveInPrepareHeader         :dword
extrn _imp_waveInReset                 :dword
extrn _imp_waveInStart                 :dword
extrn _imp_waveInStop                  :dword
extrn _imp_waveInUnprepareHeader       :dword
extrn _imp_waveOutBreakLoop            :dword
extrn _imp_waveOutClose                :dword
extrn _imp_waveOutGetDevCapsA          :dword
extrn _imp_waveOutGetDevCapsW          :dword
extrn _imp_waveOutGetErrorTextA        :dword
extrn _imp_waveOutGetErrorTextW        :dword
extrn _imp_waveOutGetID                :dword
extrn _imp_waveOutGetNumDevs           :dword
extrn _imp_waveOutGetPitch             :dword
extrn _imp_waveOutGetPlaybackRate      :dword
extrn _imp_waveOutGetPosition          :dword
extrn _imp_waveOutGetVolume            :dword
extrn _imp_waveOutMessage              :dword
extrn _imp_waveOutOpen                 :dword
extrn _imp_waveOutPause                :dword
extrn _imp_waveOutPrepareHeader        :dword
extrn _imp_waveOutReset                :dword
extrn _imp_waveOutRestart              :dword
extrn _imp_waveOutSetPitch             :dword
extrn _imp_waveOutSetPlaybackRate      :dword
extrn _imp_waveOutSetVolume            :dword
extrn _imp_waveOutUnprepareHeader      :dword
extrn _imp_waveOutWrite                :dword
extrn _imp_wid32Message                :dword
extrn _imp_winmmDbgOut                 :dword
extrn _imp_winmmSetDebugLevel          :dword
extrn _imp_wod32Message                :dword

public _exp_NONAME0
public _exp_CloseDriver
public _exp_DefDriverProc
public _exp_DriverCallback
public _exp_DrvGetModuleHandle
public _exp_GetDriverModuleHandle
public _exp_MigrateAllDrivers
public _exp_MigrateMidiUser
public _exp_MigrateSoundEvents
public _exp_NotifyCallbackData
public _exp_OpenDriver
public _exp_PlaySound
public _exp_PlaySoundA
public _exp_PlaySoundW
public _exp_SendDriverMessage
public _exp_WOW32DriverCallback
public _exp_WOW32ResolveMultiMediaHandle
public _exp_WOWAppExit
public _exp_WinmmLogoff
public _exp_WinmmLogon
public _exp_aux32Message
public _exp_auxGetDevCapsA
public _exp_auxGetDevCapsW
public _exp_auxGetNumDevs
public _exp_auxGetVolume
public _exp_auxOutMessage
public _exp_auxSetVolume
public _exp_joy32Message
public _exp_joyConfigChanged
public _exp_joyGetDevCapsA
public _exp_joyGetDevCapsW
public _exp_joyGetNumDevs
public _exp_joyGetPos
public _exp_joyGetPosEx
public _exp_joyGetThreshold
public _exp_joyReleaseCapture
public _exp_joySetCapture
public _exp_joySetThreshold
public _exp_mci32Message
public _exp_mciDriverNotify
public _exp_mciDriverYield
public _exp_mciExecute
public _exp_mciFreeCommandResource
public _exp_mciGetCreatorTask
public _exp_mciGetDeviceIDA
public _exp_mciGetDeviceIDFromElementIDA
public _exp_mciGetDeviceIDFromElementIDW
public _exp_mciGetDeviceIDW
public _exp_mciGetDriverData
public _exp_mciGetErrorStringA
public _exp_mciGetErrorStringW
public _exp_mciGetYieldProc
public _exp_mciLoadCommandResource
public _exp_mciSendCommandA
public _exp_mciSendCommandW
public _exp_mciSendStringA
public _exp_mciSendStringW
public _exp_mciSetDriverData
public _exp_mciSetYieldProc
public _exp_mid32Message
public _exp_midiConnect
public _exp_midiDisconnect
public _exp_midiInAddBuffer
public _exp_midiInClose
public _exp_midiInGetDevCapsA
public _exp_midiInGetDevCapsW
public _exp_midiInGetErrorTextA
public _exp_midiInGetErrorTextW
public _exp_midiInGetID
public _exp_midiInGetNumDevs
public _exp_midiInMessage
public _exp_midiInOpen
public _exp_midiInPrepareHeader
public _exp_midiInReset
public _exp_midiInStart
public _exp_midiInStop
public _exp_midiInUnprepareHeader
public _exp_midiOutCacheDrumPatches
public _exp_midiOutCachePatches
public _exp_midiOutClose
public _exp_midiOutGetDevCapsA
public _exp_midiOutGetDevCapsW
public _exp_midiOutGetErrorTextA
public _exp_midiOutGetErrorTextW
public _exp_midiOutGetID
public _exp_midiOutGetNumDevs
public _exp_midiOutGetVolume
public _exp_midiOutLongMsg
public _exp_midiOutMessage
public _exp_midiOutOpen
public _exp_midiOutPrepareHeader
public _exp_midiOutReset
public _exp_midiOutSetVolume
public _exp_midiOutShortMsg
public _exp_midiOutUnprepareHeader
public _exp_midiStreamClose
public _exp_midiStreamOpen
public _exp_midiStreamOut
public _exp_midiStreamPause
public _exp_midiStreamPosition
public _exp_midiStreamProperty
public _exp_midiStreamRestart
public _exp_midiStreamStop
public _exp_mixerClose
public _exp_mixerGetControlDetailsA
public _exp_mixerGetControlDetailsW
public _exp_mixerGetDevCapsA
public _exp_mixerGetDevCapsW
public _exp_mixerGetID
public _exp_mixerGetLineControlsA
public _exp_mixerGetLineControlsW
public _exp_mixerGetLineInfoA
public _exp_mixerGetLineInfoW
public _exp_mixerGetNumDevs
public _exp_mixerMessage
public _exp_mixerOpen
public _exp_mixerSetControlDetails
public _exp_mmDrvInstall
public _exp_mmGetCurrentTask
public _exp_mmTaskBlock
public _exp_mmTaskCreate
public _exp_mmTaskSignal
public _exp_mmTaskYield
public _exp_mmioAdvance
public _exp_mmioAscend
public _exp_mmioClose
public _exp_mmioCreateChunk
public _exp_mmioDescend
public _exp_mmioFlush
public _exp_mmioGetInfo
public _exp_mmioInstallIOProcA
public _exp_mmioInstallIOProcW
public _exp_mmioOpenA
public _exp_mmioOpenW
public _exp_mmioRead
public _exp_mmioRenameA
public _exp_mmioRenameW
public _exp_mmioSeek
public _exp_mmioSendMessage
public _exp_mmioSetBuffer
public _exp_mmioSetInfo
public _exp_mmioStringToFOURCCA
public _exp_mmioStringToFOURCCW
public _exp_mmioWrite
public _exp_mmsystemGetVersion
public _exp_mod32Message
public _exp_mxd32Message
public _exp_sndPlaySoundA
public _exp_sndPlaySoundW
public _exp_tid32Message
public _exp_timeBeginPeriod
public _exp_timeEndPeriod
public _exp_timeGetDevCaps
public _exp_timeGetSystemTime
public _exp_timeGetTime
public _exp_timeKillEvent
public _exp_timeSetEvent
public _exp_waveInAddBuffer
public _exp_waveInClose
public _exp_waveInGetDevCapsA
public _exp_waveInGetDevCapsW
public _exp_waveInGetErrorTextA
public _exp_waveInGetErrorTextW
public _exp_waveInGetID
public _exp_waveInGetNumDevs
public _exp_waveInGetPosition
public _exp_waveInMessage
public _exp_waveInOpen
public _exp_waveInPrepareHeader
public _exp_waveInReset
public _exp_waveInStart
public _exp_waveInStop
public _exp_waveInUnprepareHeader
public _exp_waveOutBreakLoop
public _exp_waveOutClose
public _exp_waveOutGetDevCapsA
public _exp_waveOutGetDevCapsW
public _exp_waveOutGetErrorTextA
public _exp_waveOutGetErrorTextW
public _exp_waveOutGetID
public _exp_waveOutGetNumDevs
public _exp_waveOutGetPitch
public _exp_waveOutGetPlaybackRate
public _exp_waveOutGetPosition
public _exp_waveOutGetVolume
public _exp_waveOutMessage
public _exp_waveOutOpen
public _exp_waveOutPause
public _exp_waveOutPrepareHeader
public _exp_waveOutReset
public _exp_waveOutRestart
public _exp_waveOutSetPitch
public _exp_waveOutSetPlaybackRate
public _exp_waveOutSetVolume
public _exp_waveOutUnprepareHeader
public _exp_waveOutWrite
public _exp_wid32Message
public _exp_winmmDbgOut
public _exp_winmmSetDebugLevel
public _exp_wod32Message

.code

JumpWithTwoBreakPoint macro Address
	jmp     Address
	int     3
	int     3
endm

align 16

_exp_NONAME0                     : JumpWithTwoBreakPoint _imp_NONAME0
_exp_CloseDriver                 : JumpWithTwoBreakPoint _imp_CloseDriver
_exp_DefDriverProc               : JumpWithTwoBreakPoint _imp_DefDriverProc
_exp_DriverCallback              : JumpWithTwoBreakPoint _imp_DriverCallback
_exp_DrvGetModuleHandle          : JumpWithTwoBreakPoint _imp_DrvGetModuleHandle
_exp_GetDriverModuleHandle       : JumpWithTwoBreakPoint _imp_GetDriverModuleHandle
_exp_MigrateAllDrivers           : JumpWithTwoBreakPoint _imp_MigrateAllDrivers
_exp_MigrateMidiUser             : JumpWithTwoBreakPoint _imp_MigrateMidiUser
_exp_MigrateSoundEvents          : JumpWithTwoBreakPoint _imp_MigrateSoundEvents
_exp_NotifyCallbackData          : JumpWithTwoBreakPoint _imp_NotifyCallbackData
_exp_OpenDriver                  : JumpWithTwoBreakPoint _imp_OpenDriver
_exp_PlaySound                   : JumpWithTwoBreakPoint _imp_PlaySound
_exp_PlaySoundA                  : JumpWithTwoBreakPoint _imp_PlaySoundA
_exp_PlaySoundW                  : JumpWithTwoBreakPoint _imp_PlaySoundW
_exp_SendDriverMessage           : JumpWithTwoBreakPoint _imp_SendDriverMessage
_exp_WOW32DriverCallback         : JumpWithTwoBreakPoint _imp_WOW32DriverCallback
_exp_WOW32ResolveMultiMediaHandle: JumpWithTwoBreakPoint _imp_WOW32ResolveMultiMediaHandle
_exp_WOWAppExit                  : JumpWithTwoBreakPoint _imp_WOWAppExit
_exp_WinmmLogoff                 : JumpWithTwoBreakPoint _imp_WinmmLogoff
_exp_WinmmLogon                  : JumpWithTwoBreakPoint _imp_WinmmLogon
_exp_aux32Message                : JumpWithTwoBreakPoint _imp_aux32Message
_exp_auxGetDevCapsA              : JumpWithTwoBreakPoint _imp_auxGetDevCapsA
_exp_auxGetDevCapsW              : JumpWithTwoBreakPoint _imp_auxGetDevCapsW
_exp_auxGetNumDevs               : JumpWithTwoBreakPoint _imp_auxGetNumDevs
_exp_auxGetVolume                : JumpWithTwoBreakPoint _imp_auxGetVolume
_exp_auxOutMessage               : JumpWithTwoBreakPoint _imp_auxOutMessage
_exp_auxSetVolume                : JumpWithTwoBreakPoint _imp_auxSetVolume
_exp_joy32Message                : JumpWithTwoBreakPoint _imp_joy32Message
_exp_joyConfigChanged            : JumpWithTwoBreakPoint _imp_joyConfigChanged
_exp_joyGetDevCapsA              : JumpWithTwoBreakPoint _imp_joyGetDevCapsA
_exp_joyGetDevCapsW              : JumpWithTwoBreakPoint _imp_joyGetDevCapsW
_exp_joyGetNumDevs               : JumpWithTwoBreakPoint _imp_joyGetNumDevs
_exp_joyGetPos                   : JumpWithTwoBreakPoint _imp_joyGetPos
_exp_joyGetPosEx                 : JumpWithTwoBreakPoint _imp_joyGetPosEx
_exp_joyGetThreshold             : JumpWithTwoBreakPoint _imp_joyGetThreshold
_exp_joyReleaseCapture           : JumpWithTwoBreakPoint _imp_joyReleaseCapture
_exp_joySetCapture               : JumpWithTwoBreakPoint _imp_joySetCapture
_exp_joySetThreshold             : JumpWithTwoBreakPoint _imp_joySetThreshold
_exp_mci32Message                : JumpWithTwoBreakPoint _imp_mci32Message
_exp_mciDriverNotify             : JumpWithTwoBreakPoint _imp_mciDriverNotify
_exp_mciDriverYield              : JumpWithTwoBreakPoint _imp_mciDriverYield
_exp_mciExecute                  : JumpWithTwoBreakPoint _imp_mciExecute
_exp_mciFreeCommandResource      : JumpWithTwoBreakPoint _imp_mciFreeCommandResource
_exp_mciGetCreatorTask           : JumpWithTwoBreakPoint _imp_mciGetCreatorTask
_exp_mciGetDeviceIDA             : JumpWithTwoBreakPoint _imp_mciGetDeviceIDA
_exp_mciGetDeviceIDFromElementIDA: JumpWithTwoBreakPoint _imp_mciGetDeviceIDFromElementIDA
_exp_mciGetDeviceIDFromElementIDW: JumpWithTwoBreakPoint _imp_mciGetDeviceIDFromElementIDW
_exp_mciGetDeviceIDW             : JumpWithTwoBreakPoint _imp_mciGetDeviceIDW
_exp_mciGetDriverData            : JumpWithTwoBreakPoint _imp_mciGetDriverData
_exp_mciGetErrorStringA          : JumpWithTwoBreakPoint _imp_mciGetErrorStringA
_exp_mciGetErrorStringW          : JumpWithTwoBreakPoint _imp_mciGetErrorStringW
_exp_mciGetYieldProc             : JumpWithTwoBreakPoint _imp_mciGetYieldProc
_exp_mciLoadCommandResource      : JumpWithTwoBreakPoint _imp_mciLoadCommandResource
_exp_mciSendCommandA             : JumpWithTwoBreakPoint _imp_mciSendCommandA
_exp_mciSendCommandW             : JumpWithTwoBreakPoint _imp_mciSendCommandW
_exp_mciSendStringA              : JumpWithTwoBreakPoint _imp_mciSendStringA
_exp_mciSendStringW              : JumpWithTwoBreakPoint _imp_mciSendStringW
_exp_mciSetDriverData            : JumpWithTwoBreakPoint _imp_mciSetDriverData
_exp_mciSetYieldProc             : JumpWithTwoBreakPoint _imp_mciSetYieldProc
_exp_mid32Message                : JumpWithTwoBreakPoint _imp_mid32Message
_exp_midiConnect                 : JumpWithTwoBreakPoint _imp_midiConnect
_exp_midiDisconnect              : JumpWithTwoBreakPoint _imp_midiDisconnect
_exp_midiInAddBuffer             : JumpWithTwoBreakPoint _imp_midiInAddBuffer
_exp_midiInClose                 : JumpWithTwoBreakPoint _imp_midiInClose
_exp_midiInGetDevCapsA           : JumpWithTwoBreakPoint _imp_midiInGetDevCapsA
_exp_midiInGetDevCapsW           : JumpWithTwoBreakPoint _imp_midiInGetDevCapsW
_exp_midiInGetErrorTextA         : JumpWithTwoBreakPoint _imp_midiInGetErrorTextA
_exp_midiInGetErrorTextW         : JumpWithTwoBreakPoint _imp_midiInGetErrorTextW
_exp_midiInGetID                 : JumpWithTwoBreakPoint _imp_midiInGetID
_exp_midiInGetNumDevs            : JumpWithTwoBreakPoint _imp_midiInGetNumDevs
_exp_midiInMessage               : JumpWithTwoBreakPoint _imp_midiInMessage
_exp_midiInOpen                  : JumpWithTwoBreakPoint _imp_midiInOpen
_exp_midiInPrepareHeader         : JumpWithTwoBreakPoint _imp_midiInPrepareHeader
_exp_midiInReset                 : JumpWithTwoBreakPoint _imp_midiInReset
_exp_midiInStart                 : JumpWithTwoBreakPoint _imp_midiInStart
_exp_midiInStop                  : JumpWithTwoBreakPoint _imp_midiInStop
_exp_midiInUnprepareHeader       : JumpWithTwoBreakPoint _imp_midiInUnprepareHeader
_exp_midiOutCacheDrumPatches     : JumpWithTwoBreakPoint _imp_midiOutCacheDrumPatches
_exp_midiOutCachePatches         : JumpWithTwoBreakPoint _imp_midiOutCachePatches
_exp_midiOutClose                : JumpWithTwoBreakPoint _imp_midiOutClose
_exp_midiOutGetDevCapsA          : JumpWithTwoBreakPoint _imp_midiOutGetDevCapsA
_exp_midiOutGetDevCapsW          : JumpWithTwoBreakPoint _imp_midiOutGetDevCapsW
_exp_midiOutGetErrorTextA        : JumpWithTwoBreakPoint _imp_midiOutGetErrorTextA
_exp_midiOutGetErrorTextW        : JumpWithTwoBreakPoint _imp_midiOutGetErrorTextW
_exp_midiOutGetID                : JumpWithTwoBreakPoint _imp_midiOutGetID
_exp_midiOutGetNumDevs           : JumpWithTwoBreakPoint _imp_midiOutGetNumDevs
_exp_midiOutGetVolume            : JumpWithTwoBreakPoint _imp_midiOutGetVolume
_exp_midiOutLongMsg              : JumpWithTwoBreakPoint _imp_midiOutLongMsg
_exp_midiOutMessage              : JumpWithTwoBreakPoint _imp_midiOutMessage
_exp_midiOutOpen                 : JumpWithTwoBreakPoint _imp_midiOutOpen
_exp_midiOutPrepareHeader        : JumpWithTwoBreakPoint _imp_midiOutPrepareHeader
_exp_midiOutReset                : JumpWithTwoBreakPoint _imp_midiOutReset
_exp_midiOutSetVolume            : JumpWithTwoBreakPoint _imp_midiOutSetVolume
_exp_midiOutShortMsg             : JumpWithTwoBreakPoint _imp_midiOutShortMsg
_exp_midiOutUnprepareHeader      : JumpWithTwoBreakPoint _imp_midiOutUnprepareHeader
_exp_midiStreamClose             : JumpWithTwoBreakPoint _imp_midiStreamClose
_exp_midiStreamOpen              : JumpWithTwoBreakPoint _imp_midiStreamOpen
_exp_midiStreamOut               : JumpWithTwoBreakPoint _imp_midiStreamOut
_exp_midiStreamPause             : JumpWithTwoBreakPoint _imp_midiStreamPause
_exp_midiStreamPosition          : JumpWithTwoBreakPoint _imp_midiStreamPosition
_exp_midiStreamProperty          : JumpWithTwoBreakPoint _imp_midiStreamProperty
_exp_midiStreamRestart           : JumpWithTwoBreakPoint _imp_midiStreamRestart
_exp_midiStreamStop              : JumpWithTwoBreakPoint _imp_midiStreamStop
_exp_mixerClose                  : JumpWithTwoBreakPoint _imp_mixerClose
_exp_mixerGetControlDetailsA     : JumpWithTwoBreakPoint _imp_mixerGetControlDetailsA
_exp_mixerGetControlDetailsW     : JumpWithTwoBreakPoint _imp_mixerGetControlDetailsW
_exp_mixerGetDevCapsA            : JumpWithTwoBreakPoint _imp_mixerGetDevCapsA
_exp_mixerGetDevCapsW            : JumpWithTwoBreakPoint _imp_mixerGetDevCapsW
_exp_mixerGetID                  : JumpWithTwoBreakPoint _imp_mixerGetID
_exp_mixerGetLineControlsA       : JumpWithTwoBreakPoint _imp_mixerGetLineControlsA
_exp_mixerGetLineControlsW       : JumpWithTwoBreakPoint _imp_mixerGetLineControlsW
_exp_mixerGetLineInfoA           : JumpWithTwoBreakPoint _imp_mixerGetLineInfoA
_exp_mixerGetLineInfoW           : JumpWithTwoBreakPoint _imp_mixerGetLineInfoW
_exp_mixerGetNumDevs             : JumpWithTwoBreakPoint _imp_mixerGetNumDevs
_exp_mixerMessage                : JumpWithTwoBreakPoint _imp_mixerMessage
_exp_mixerOpen                   : JumpWithTwoBreakPoint _imp_mixerOpen
_exp_mixerSetControlDetails      : JumpWithTwoBreakPoint _imp_mixerSetControlDetails
_exp_mmDrvInstall                : JumpWithTwoBreakPoint _imp_mmDrvInstall
_exp_mmGetCurrentTask            : JumpWithTwoBreakPoint _imp_mmGetCurrentTask
_exp_mmTaskBlock                 : JumpWithTwoBreakPoint _imp_mmTaskBlock
_exp_mmTaskCreate                : JumpWithTwoBreakPoint _imp_mmTaskCreate
_exp_mmTaskSignal                : JumpWithTwoBreakPoint _imp_mmTaskSignal
_exp_mmTaskYield                 : JumpWithTwoBreakPoint _imp_mmTaskYield
_exp_mmioAdvance                 : JumpWithTwoBreakPoint _imp_mmioAdvance
_exp_mmioAscend                  : JumpWithTwoBreakPoint _imp_mmioAscend
_exp_mmioClose                   : JumpWithTwoBreakPoint _imp_mmioClose
_exp_mmioCreateChunk             : JumpWithTwoBreakPoint _imp_mmioCreateChunk
_exp_mmioDescend                 : JumpWithTwoBreakPoint _imp_mmioDescend
_exp_mmioFlush                   : JumpWithTwoBreakPoint _imp_mmioFlush
_exp_mmioGetInfo                 : JumpWithTwoBreakPoint _imp_mmioGetInfo
_exp_mmioInstallIOProcA          : JumpWithTwoBreakPoint _imp_mmioInstallIOProcA
_exp_mmioInstallIOProcW          : JumpWithTwoBreakPoint _imp_mmioInstallIOProcW
_exp_mmioOpenA                   : JumpWithTwoBreakPoint _imp_mmioOpenA
_exp_mmioOpenW                   : JumpWithTwoBreakPoint _imp_mmioOpenW
_exp_mmioRead                    : JumpWithTwoBreakPoint _imp_mmioRead
_exp_mmioRenameA                 : JumpWithTwoBreakPoint _imp_mmioRenameA
_exp_mmioRenameW                 : JumpWithTwoBreakPoint _imp_mmioRenameW
_exp_mmioSeek                    : JumpWithTwoBreakPoint _imp_mmioSeek
_exp_mmioSendMessage             : JumpWithTwoBreakPoint _imp_mmioSendMessage
_exp_mmioSetBuffer               : JumpWithTwoBreakPoint _imp_mmioSetBuffer
_exp_mmioSetInfo                 : JumpWithTwoBreakPoint _imp_mmioSetInfo
_exp_mmioStringToFOURCCA         : JumpWithTwoBreakPoint _imp_mmioStringToFOURCCA
_exp_mmioStringToFOURCCW         : JumpWithTwoBreakPoint _imp_mmioStringToFOURCCW
_exp_mmioWrite                   : JumpWithTwoBreakPoint _imp_mmioWrite
_exp_mmsystemGetVersion          : JumpWithTwoBreakPoint _imp_mmsystemGetVersion
_exp_mod32Message                : JumpWithTwoBreakPoint _imp_mod32Message
_exp_mxd32Message                : JumpWithTwoBreakPoint _imp_mxd32Message
_exp_sndPlaySoundA               : JumpWithTwoBreakPoint _imp_sndPlaySoundA
_exp_sndPlaySoundW               : JumpWithTwoBreakPoint _imp_sndPlaySoundW
_exp_tid32Message                : JumpWithTwoBreakPoint _imp_tid32Message
_exp_timeBeginPeriod             : JumpWithTwoBreakPoint _imp_timeBeginPeriod
_exp_timeEndPeriod               : JumpWithTwoBreakPoint _imp_timeEndPeriod
_exp_timeGetDevCaps              : JumpWithTwoBreakPoint _imp_timeGetDevCaps
_exp_timeGetSystemTime           : JumpWithTwoBreakPoint _imp_timeGetSystemTime
_exp_timeGetTime                 : JumpWithTwoBreakPoint _imp_timeGetTime
_exp_timeKillEvent               : JumpWithTwoBreakPoint _imp_timeKillEvent
_exp_timeSetEvent                : JumpWithTwoBreakPoint _imp_timeSetEvent
_exp_waveInAddBuffer             : JumpWithTwoBreakPoint _imp_waveInAddBuffer
_exp_waveInClose                 : JumpWithTwoBreakPoint _imp_waveInClose
_exp_waveInGetDevCapsA           : JumpWithTwoBreakPoint _imp_waveInGetDevCapsA
_exp_waveInGetDevCapsW           : JumpWithTwoBreakPoint _imp_waveInGetDevCapsW
_exp_waveInGetErrorTextA         : JumpWithTwoBreakPoint _imp_waveInGetErrorTextA
_exp_waveInGetErrorTextW         : JumpWithTwoBreakPoint _imp_waveInGetErrorTextW
_exp_waveInGetID                 : JumpWithTwoBreakPoint _imp_waveInGetID
_exp_waveInGetNumDevs            : JumpWithTwoBreakPoint _imp_waveInGetNumDevs
_exp_waveInGetPosition           : JumpWithTwoBreakPoint _imp_waveInGetPosition
_exp_waveInMessage               : JumpWithTwoBreakPoint _imp_waveInMessage
_exp_waveInOpen                  : JumpWithTwoBreakPoint _imp_waveInOpen
_exp_waveInPrepareHeader         : JumpWithTwoBreakPoint _imp_waveInPrepareHeader
_exp_waveInReset                 : JumpWithTwoBreakPoint _imp_waveInReset
_exp_waveInStart                 : JumpWithTwoBreakPoint _imp_waveInStart
_exp_waveInStop                  : JumpWithTwoBreakPoint _imp_waveInStop
_exp_waveInUnprepareHeader       : JumpWithTwoBreakPoint _imp_waveInUnprepareHeader
_exp_waveOutBreakLoop            : JumpWithTwoBreakPoint _imp_waveOutBreakLoop
_exp_waveOutClose                : JumpWithTwoBreakPoint _imp_waveOutClose
_exp_waveOutGetDevCapsA          : JumpWithTwoBreakPoint _imp_waveOutGetDevCapsA
_exp_waveOutGetDevCapsW          : JumpWithTwoBreakPoint _imp_waveOutGetDevCapsW
_exp_waveOutGetErrorTextA        : JumpWithTwoBreakPoint _imp_waveOutGetErrorTextA
_exp_waveOutGetErrorTextW        : JumpWithTwoBreakPoint _imp_waveOutGetErrorTextW
_exp_waveOutGetID                : JumpWithTwoBreakPoint _imp_waveOutGetID
_exp_waveOutGetNumDevs           : JumpWithTwoBreakPoint _imp_waveOutGetNumDevs
_exp_waveOutGetPitch             : JumpWithTwoBreakPoint _imp_waveOutGetPitch
_exp_waveOutGetPlaybackRate      : JumpWithTwoBreakPoint _imp_waveOutGetPlaybackRate
_exp_waveOutGetPosition          : JumpWithTwoBreakPoint _imp_waveOutGetPosition
_exp_waveOutGetVolume            : JumpWithTwoBreakPoint _imp_waveOutGetVolume
_exp_waveOutMessage              : JumpWithTwoBreakPoint _imp_waveOutMessage
_exp_waveOutOpen                 : JumpWithTwoBreakPoint _imp_waveOutOpen
_exp_waveOutPause                : JumpWithTwoBreakPoint _imp_waveOutPause
_exp_waveOutPrepareHeader        : JumpWithTwoBreakPoint _imp_waveOutPrepareHeader
_exp_waveOutReset                : JumpWithTwoBreakPoint _imp_waveOutReset
_exp_waveOutRestart              : JumpWithTwoBreakPoint _imp_waveOutRestart
_exp_waveOutSetPitch             : JumpWithTwoBreakPoint _imp_waveOutSetPitch
_exp_waveOutSetPlaybackRate      : JumpWithTwoBreakPoint _imp_waveOutSetPlaybackRate
_exp_waveOutSetVolume            : JumpWithTwoBreakPoint _imp_waveOutSetVolume
_exp_waveOutUnprepareHeader      : JumpWithTwoBreakPoint _imp_waveOutUnprepareHeader
_exp_waveOutWrite                : JumpWithTwoBreakPoint _imp_waveOutWrite
_exp_wid32Message                : JumpWithTwoBreakPoint _imp_wid32Message
_exp_winmmDbgOut                 : JumpWithTwoBreakPoint _imp_winmmDbgOut
_exp_winmmSetDebugLevel          : JumpWithTwoBreakPoint _imp_winmmSetDebugLevel
_exp_wod32Message                : JumpWithTwoBreakPoint _imp_wod32Message

end
