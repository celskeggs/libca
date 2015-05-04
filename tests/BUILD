[cc_test(
	name = f[:-2],
	srcs = [f],
	copts = ["-nostdlib", "-nostdinc"],
	linkopts = ["-nostdlib", "-nostdinc"],
	deps = ["//libca/include:includes"],
) for f in glob(["**/*.c"])]
test_suite(
	name = "tests",
)
