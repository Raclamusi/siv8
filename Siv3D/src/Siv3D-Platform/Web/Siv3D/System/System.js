addToLibrary({
	siv3dWaitForAnimationFrame__async: "auto",
	siv3dWaitForAnimationFrame: (vSyncEnabled) => {
		if (vSyncEnabled) {
			return new Promise(resolve => {
				requestAnimationFrame(() => resolve());
			});
		}
		else {
			return Promise.resolve();
		}
	},

	siv3dLaunchBrowser__deps: ["$siv3dUserActivation", "$UTF32ToString", "$FS"],
	siv3dLaunchBrowser: (ptr, length) => {
		const url = UTF32ToString(ptr, length * 4, false);

		if (url.startsWith("/") && FS.analyzePath(url).exists) {
			const html = FS.readFile(url, { encoding: "utf8" });
			siv3dUserActivation.wait().then(() => {
				open("", "_blank").document.documentElement.innerHTML = html;
			});
		}
		else {
			siv3dUserActivation.wait().then(() => {
				open(url, "_blank");
			});
		}
	},

	$siv3dUserActivation__postset: () => {
		addAtInit("siv3dUserActivation.init();");
		addAtExit("siv3dUserActivation.quit();");
	},
	$siv3dUserActivation: {
		resolvers: [],
		wait: () => {
			if (navigator.userActivation.isActive) {
				return Promise.resolve();
			}
			return new Promise(resolve => {
				siv3dUserActivation.resolvers.push(resolve);
			});
		},
		resolve: () => {
			if (navigator.userActivation.isActive) {
				for (const resolve of siv3dUserActivation.resolvers) {
					resolve();
				}
				siv3dUserActivation.resolvers.splice(0);
			}
		},
		init: () => {
			addEventListener('keydown', siv3dUserActivation.resolve);
			addEventListener('mousedown', siv3dUserActivation.resolve);
			addEventListener('touchend', siv3dUserActivation.resolve);
		},
		quit: () => {
			removeEventListener('keydown', siv3dUserActivation.resolve);
			removeEventListener('mousedown', siv3dUserActivation.resolve);
			removeEventListener('touchend', siv3dUserActivation.resolve);
		},
	},
});
