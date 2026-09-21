addToLibrary({
	siv3dShowMessageBoxOK_deps: ["$UTF32ToString"],
	siv3dShowMessageBoxOK: (ptr, length) => {
		const text = UTF32ToString(ptr, length * 4, false);
		alert(text);
	},

	siv3dShowMessageBoxOKCancel_deps: ["$UTF32ToString"],
	siv3dShowMessageBoxOKCancel: (ptr, length) => {
		const text = UTF32ToString(ptr, length * 4, false);
		return confirm(text);
	},
});
