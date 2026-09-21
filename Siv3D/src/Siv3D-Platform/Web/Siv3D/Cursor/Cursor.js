addToLibrary({
	$siv3dCursor__postset: () => {
		addAtInit("siv3dCursor.init();");
		addAtExit("siv3dCursor.quit();");
	},
	$siv3dCursor: {
		pos: [0, 0],
		onPointerChange: (event) => {
			if (event.isPrimary) {
				siv3dCursor.pos[0] = event.screenX * devicePixelRatio;
				siv3dCursor.pos[1] = event.screenY * devicePixelRatio;
			}
		},
		init: () => {
			addEventListener("pointerdown", siv3dCursor.onPointerChange);
			addEventListener("pointermove", siv3dCursor.onPointerChange);
		},
		quit: () => {
			removeEventListener("pointerdown", siv3dCursor.onPointerChange);
			removeEventListener("pointermove", siv3dCursor.onPointerChange);
		},
	},

	siv3dGetCursorPos__deps: ["$siv3dCursor"],
	siv3dGetCursorPos: (xPtr, yPtr) => {
		{{{ makeSetValue("xPtr", 0, "siv3dCursor.pos[0]", "double") }}};
		{{{ makeSetValue("yPtr", 0, "siv3dCursor.pos[1]", "double") }}};
	},
});
