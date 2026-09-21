addToLibrary({
	siv3dRequestFullscreen__deps: ["$siv3dUserActivation", "$Browser"],
	siv3dRequestFullscreen: () => {
		const canvasContainer = Module["canvas"].parentElement;
		if (canvasContainerElement.requestFullscreen && document.fullscreenElement === null) {
			siv3dUserActivation.wait().then(() => {
				canvasContainer.requestFullscreen();
			});
		}
	},

	siv3dExitFullscreen__deps: ["$Browser"],
	siv3dExitFullscreen: () => {
		if (document.exitFullscreen && document.fullscreenElement !== null) {
			document.exitFullscreen();
		}
	},

	siv3dIsFullscreen: () => {
		return !!document.fullscreenElement;
	},

	siv3dGetWindowBounds: (rectPtr) => {
		const canvas = Module["canvas"];
		const rect = canvas.getBoundingClientRect();
		const x = Math.floor((screenX + rect.left) * devicePixelRatio);
		const y = Math.floor((screenY + rect.top) * devicePixelRatio);
		const w = Math.floor(rect.width * devicePixelRatio);
		const h = Math.floor(rect.height * devicePixelRatio);
		{{{ makeSetValue("rectPtr", 0, "x", "i32") }}};
		{{{ makeSetValue("rectPtr", 4, "y", "i32") }}};
		{{{ makeSetValue("rectPtr", 8, "w", "i32") }}};
		{{{ makeSetValue("rectPtr", 12, "h", "i32") }}};
	},
});
