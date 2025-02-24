@echo off

rem Step 1: Generate input files
for %%n in (50 100 200 400 800 1600) do (
  for %%l in (1 4) do (
    for %%m in (0 1) do (
      for %%s in (1 10 100) do (
        bin\generater.exe input\input_%%n_%%l_1_1000_%%m_%%s.sp %%n %%l 1 1000 %%m %%s
        bin\generater.exe input\input_%%n_%%l_1000_10000_%%m_%%s.sp %%n %%l 1000 10000 %%m %%s
      )
    )
  )
)
