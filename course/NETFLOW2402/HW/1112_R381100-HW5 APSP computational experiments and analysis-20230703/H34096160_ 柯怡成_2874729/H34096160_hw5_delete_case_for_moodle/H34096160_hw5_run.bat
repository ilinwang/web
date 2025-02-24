@echo off
rem Step 2: Run algorithms on input files
for %%f in (input\*.sp) do (
  bin\bf.exe <"%%f"> "output\%%~nf_bf.txt"
  bin\bfp.exe <"%%f"> "output\%%~nf_bfp.txt"
  bin\dikb.exe <"%%f"> "output\%%~nf_dikb.txt"
  bin\dikh.exe <"%%f"> "output\%%~nf_dikh.txt"
  bin\fwa.exe "%%f" "output\%%~nf_fwa.txt"
  bin\fwg.exe "%%f" "output\%%~nf_fwg.txt"
)