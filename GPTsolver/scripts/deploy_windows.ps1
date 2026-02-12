$ErrorActionPreference = 'Stop'
Write-Host "[1/6] 检查 winget..."
if (-not (Get-Command winget -ErrorAction SilentlyContinue)) { throw "未检测到 winget，请先安装 App Installer" }

Write-Host "[2/6] 安装 CMake/Ninja/Git..."
winget install Kitware.CMake --silent --accept-package-agreements --accept-source-agreements
winget install Ninja-build.Ninja --silent --accept-package-agreements --accept-source-agreements
winget install Git.Git --silent --accept-package-agreements --accept-source-agreements

Write-Host "[3/6] 检查 MSVC Build Tools..."
Write-Host "请确认已安装 VS2022 C++ 工具链（若未安装，请先安装 Visual Studio Build Tools）"

Write-Host "[4/6] 配置与构建..."
cmake --preset default
cmake --build --preset default -j 8

Write-Host "[5/6] 运行测试..."
ctest --test-dir build/default --output-on-failure

Write-Host "[6/6] 冒烟运行..."
.\build\default\gptsolver.exe run examples\inp\static\coupled_plate.inp --out output\deploy_windows --threads 8
Write-Host "完成：部署与冒烟测试成功。"
