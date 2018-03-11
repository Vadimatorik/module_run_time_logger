ifndef MODULE_RUN_TIME_LOGGER_OPTIMIZATION
	MODULE_RUN_TIME_LOGGER_OPTIMIZATION = -g3 -O0
endif

RUN_TIME_LOGGER_CPP_FILE		:= $(shell find module_run_time_logger/ -maxdepth 3 -type f -name "*.cpp" )
RUN_TIME_LOGGER_DIR				:= $(shell find module_run_time_logger/ -maxdepth 3 -type d -name "*" )
RUN_TIME_LOGGER_PATH			:= $(addprefix -I, $(RUN_TIME_LOGGER_DIR))
RUN_TIME_LOGGER_OBJ_FILE		:= $(addprefix build/obj/, $(RUN_TIME_LOGGER_CPP_FILE))
RUN_TIME_LOGGER_OBJ_FILE		:= $(patsubst %.cpp, %.o, $(RUN_TIME_LOGGER_OBJ_FILE))

build/obj/module_run_time_logger/%.o:	module_run_time_logger/%.cpp
	@echo [CPP] $<
	@mkdir -p $(dir $@)
	@$(CPP) $(CPP_FLAGS) $(DEFINE_PROJ) $(USER_CFG_PATH) $(RUN_TIME_LOGGER_PATH) $(FREE_RTOS_PATH) $(MODULE_RUN_TIME_LOGGER_OPTIMIZATION) $(PROJECT_PATH) -c $< -o $@

# Добавляем к общим переменным проекта.
PROJECT_PATH			+= $(RUN_TIME_LOGGER_PATH)
PROJECT_OBJ_FILE		+= $(RUN_TIME_LOGGER_OBJ_FILE)