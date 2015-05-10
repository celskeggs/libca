#include <panic.h>
#include <stream.h>
#include <libsyscall.h>

static void _panic_do(error err) __attribute__((noreturn));

void exit(u8 code) {
	_panic_do((error) {ERR_SYSEXIT, code, "program exit"});
}

void panic_static(string str) {
	_panic_do((error) {ERR_STATIC, 127, str});
}

void panic(string str) {
	_panic_do((error) {ERR_MALLOCED, 127, strdup(str)});
}

// TODO: make this thread-local
struct recover_stack_entry {
	struct lsc_saved_registers regs;
	error *ret_error;
};

static struct recover_stack_entry *stack_top = NULL;
static bool is_panicing = false;

void _panic_do(error err) {
	if (stack_top == NULL) { // oops! abort! abort!
		if (is_panicing && err.type != ERR_SYSEXIT) {
			const char err[] = "top-level exception while handling top-level exception\n";
			lsc_write(2, err, sizeof(err));
			lsc_exit(err.exit_code);
		}
		is_panicing = true;
		if (err.type == ERR_SYSEXIT) {
			lsc_exit(err.exit_code); // top-level exit, so exit!
		} else if (err.type == ERR_STATIC) {
			writes(stderr, "top-level exception: ");
			writes(stderr, err.str);
			writech(stderr, '\n');
			lsc_exit(err.exit_code);
		} else if (err.type == ERR_MALLOCED) {
			writes(stderr, "top-level exception: ");
			writes(stderr, err.str);
			writech(stderr, '\n');
			free(err.str);
			lsc_exit(err.exit_code);
		} else {
			writes(stderr, "top-level exception, but invalid!\n");
			lsc_exit(err.exit_code);
		}
	} else {
		*stack_top->ret_error = err;
		lsc_register_restore(stack_top->lsc_saved_registers);
	}
}

bool recover(recover_callback cb, void *param, error *ret) {
	struct recover_stack_entry *last = stack_top;

	struct recover_stack_entry new_ent;
	new_ent.next = stack_top;
	new_ent.ret_error = ret;

	if (lsc_register_save(&new_ent->regs)) { // return after save
		cb(param); // could jump back to lsc_register_save.
		// success
		ret->type = ERR_NONE;
		ret->exit_code = 0;
		ret->str = "no error";
		stack_top = last;
		return false;
	} else {
		// error
		stack_top = last;
		return true;
	}
}
