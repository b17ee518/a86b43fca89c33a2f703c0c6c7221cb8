regasm FiddlerCOM.dll /tlb:FiddlerCOM.tlb /u
regasm FiddlerCOM.dll /tlb:FiddlerCOM.tlb

gacutil /u FiddlerCOM
gacutil /i FiddlerCOM.dll