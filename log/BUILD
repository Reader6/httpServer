cc_library(

	name="log_lib",
	srcs=["easylogging++.cc"],
	hdrs=["easylogging++.h"],
	visibility = ["//visibility:public"],

)
cc_binary(
	name="log_test",
	srcs = ["main.cc"],
	deps = ["log_lib"],
)

