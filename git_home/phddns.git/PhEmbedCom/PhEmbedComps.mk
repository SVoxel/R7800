
PhEmbedComps.dll: dlldata.obj PhEmbedCom_p.obj PhEmbedCom_i.obj
	link /dll /out:PhEmbedComps.dll /def:PhEmbedComps.def /entry:DllMain dlldata.obj PhEmbedCom_p.obj PhEmbedCom_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del PhEmbedComps.dll
	@del PhEmbedComps.lib
	@del PhEmbedComps.exp
	@del dlldata.obj
	@del PhEmbedCom_p.obj
	@del PhEmbedCom_i.obj
