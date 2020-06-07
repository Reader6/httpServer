cc_library(
    name = "sqlconn_lib",
    srcs = ["sql_connection.cpp"],
    hdrs = ["sql_connection.h"],

    #连接phtread静态库
    linkopts = ["-lmysqlclient"],
    #可见性
    visibility = ["//visibility:public"],
)
cc_library(
   name="sqlconnpool_lib",
   srcs =["sql_connection_pool.cpp"],
   hdrs =["sql_connection_pool.h"],
   deps =[":sqlconn_lib"],
   visibility = ["//visibility:public"],
				
)
cc_library(
   name = "mysql_lib",
   srcs = ["mysql_guard.cpp"],
   hdrs = ["mysql_guard.h"],
   deps = [":sqlconnpool_lib"],
   visibility = ["//visibility:public"],
		
)

