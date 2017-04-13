#!/usr/bin/awk -f

# from <project path="abi/cpp" name="android/platform/abi/cpp-4.1" groups="pdk" />
# to   <project path="abi/cpp" name="android_platform_abi_cpp-4-1" groups="pdk" />

{
	if ($1 ~ "<project") {
		gsub(/\//,"_", $3)
		gsub(/\./,"-", $3)
	}
	print $0
}
