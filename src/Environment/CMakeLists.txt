LIST(APPEND Environment_files
Environment.hh
EnvironmentAPI.hh
SelfRegistry.hh
SelfRegistry.cxx
ModuleLoadFailedException.hh
ModuleLoader.cxx
ModuleLoader.hh
ModuleRegistry.hh
ModuleRegistry.cxx
ModuleRegister.hh
ModuleRegisterBase.hh
ModuleRegisterBase.cxx
CFEnv.cxx
CFEnv.hh
CFEnvVars.hh
CFEnvVars.cxx
ConfigRegisterBase.hh
ConfigRegisterBase.cxx
ConfigRegistry.hh
ConfigRegistry.cxx
ConcreteProvider.hh
DirPaths.hh
DirPaths.cxx
ObjectProvider.hh
FactoryRegistry.hh
FactoryRegistry.cxx
DirectFileAccess.hh
DirectFileAccess.cxx
SingleBehaviorFactory.hh
SingleBehaviorFactory.ci
DirectFileWrite.hh
DirectFileWrite.cxx
Provider.hh
FactoryBase.hh
Factory.hh
Registry.ci
Registry.hh
FileHandlerInputConcrete.ci
FileHandlerInputConcrete.hh
FileHandlerInput.cxx
FileHandlerInput.hh
FileHandlerOutputConcrete.ci
FileHandlerOutputConcrete.hh
FileHandlerOutput.cxx
FileHandlerOutput.hh
)

LIST ( APPEND OPTIONAL_dirfiles CurlAccessRepository.hh	CurlAccessRepository.cxx )
IF ( CF_HAVE_CURL )
  	LIST ( APPEND Environment_files CurlAccessRepository.hh CurlAccessRepository.cxx               )
ENDIF ( CF_HAVE_CURL )

LIST(APPEND Environment_cflibs Config )

CF_ADD_KERNEL_LIBRARY ( Environment )

CF_WARN_ORPHAN_FILES()
