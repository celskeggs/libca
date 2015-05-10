[cc_test(
	name = f[:-2],
	srcs = [f],
	copts = ["-nostdlib", "-nostdinc", "-std=c99"],
	linkopts = ["-nostdlib", "-nostdinc"],
	deps = ["//libca/include"],
) for f in glob(["**/*.c"])]
test_suite(
	name = "tests",
)
