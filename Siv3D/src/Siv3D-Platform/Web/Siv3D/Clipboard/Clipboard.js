addToLibrary({
	$siv3dClipboardSequenceNumber__postset: () => {
		addAtInit("siv3dClipboardSequenceNumber.init();");
		addAtExit("siv3dClipboardSequenceNumber.quit();");
	},
	$siv3dClipboardSequenceNumber: {
		value: 0,
		onChange: () => {
			siv3dClipboardSequenceNumber.value += 1;
		},
		init: () => {
			if ("onclipboardchange" in navigator.clipboard) {
				navigator.clipboard.addEventListener("clipboardchange", siv3dClipboardSequenceNumber.onChange);
			}
		},
		quit: () => {
			if ("onclipboardchange" in navigator.clipboard) {
				navigator.clipboard.removeEventListener("clipboardchange", siv3dClipboardSequenceNumber.onChange);
			}
		},
	},

	siv3dGetClipboardSequenceNumber__deps: ["$siv3dClipboardSequenceNumber"],
	siv3dGetClipboardSequenceNumber: () => {
		return siv3dClipboardSequenceNumber.value;
	},

	siv3dSetClipboardText__deps: ["$siv3dUserActivation", "$UTF32ToString"],
	siv3dSetClipboardText: (ptr, length) => {
		const text = UTF32ToString(ptr, length * 4, false);
		siv3dUserActivation.wait().then(() => {
			navigator.clipboard.writeText(text);
		});
	},

	siv3dGetClipboardText__deps: ["$siv3dUserActivation", "$lengthBytesUTF32", "malloc", "$stringToUTF32"],
	siv3dGetClipboardText__async: "auto",
	siv3dGetClipboardText: async (lengthPtr) => {
		await siv3dUserActivation.wait();
		const text = await navigator.clipboard.readText();
		const size = lengthBytesUTF32(text) + 4;
		const ptr = _malloc(size);
		if (ptr) {
			stringToUTF32(text, ptr, size);
			const length = (size / 4) - 1;
			{{{ makeSetValue("lengthPtr", 0, "length", SIZE_TYPE) }}};
			return ptr;
		}
		return 0;
	},

	siv3dHasClipboardText__deps: ["$siv3dUserActivation"],
	siv3dHasClipboardText__async: "auto",
	siv3dHasClipboardText: async () => {
		await siv3dUserActivation.wait();
		const text = await navigator.clipboard.readText();
		return text.length > 0;
	},

	$siv3dSetClipboard__deps: ["$siv3dUserActivation", "$UTF32ToString"],
	$siv3dSetClipboard: (blob, plainTextPtr = 0, plainTextLength = 0) => {
		const clipboardData = { [blob.type]: blob };
		if (plainTextPtr) {
			const plainText = UTF32ToString(plainTextPtr, plainTextLength * 4, false);
			const textBlob = new Blob([ plainText ], { type: "text/plain" });
			clipboardData["text/plain"] = textBlob;
		}
		const clipboardItem = new ClipboardItem(clipboardData);
		siv3dUserActivation.wait().then(() => {
			navigator.clipboard.write([ clipboardItem ]);
		});
	},

	$siv3dGetClipboard__deps: ["$siv3dUserActivation", "malloc"],
	$siv3dGetClipboard: async (mimeType, dataSizePtr) => {
		await siv3dUserActivation.wait();
		const items = await navigator.clipboard.read();
		if (items.length > 0 && items[0].types.includes(mimeType)) {
			const blob = await items[0].getType(mimeType);
			const data = new Uint8Array(await blob.arrayBuffer());
			const ptr = _malloc(data.length);
			if (ptr) {
				HEAPU8.set(data, ptr);
				{{{ makeSetValue("dataSizePtr", 0, "data.length", SIZE_TYPE) }}};
				return ptr;
			}
		}
		return 0;
	},

	siv3dSetClipboardImage__deps: ["$siv3dSetClipboard"],
	siv3dSetClipboardImage: (pngPtr, pngSize) => {
		const pngData = HEAPU8.subarray(pngPtr, pngPtr + pngSize);
		const pngBlob = new Blob([ pngData ], { type: "image/png" });
		siv3dSetClipboard(pngBlob);
	},

	siv3dGetClipboardImage__deps: ["$siv3dGetClipboard"],
	siv3dGetClipboardImage__async: "auto",
	siv3dGetClipboardImage: async (pngSizePtr) => {
		return await siv3dGetClipboard("image/png", pngSizePtr);
	},

	siv3dHasClipboardImage__deps: ["$siv3dUserActivation"],
	siv3dHasClipboardImage__async: "auto",
	siv3dHasClipboardImage: async () => {
		await siv3dUserActivation.wait();
		const items = await navigator.clipboard.read();
		return items.length > 0 && items[0].types.includes("image/png");
	},

	siv3dSetClipboardHTML__deps: ["$siv3dSetClipboard", "$UTF32ToString"],
	siv3dSetClipboardHTML: (htmlPtr, htmlLength, plainTextPtr, plainTextLength) => {
		const html = UTF32ToString(htmlPtr, htmlLength * 4, false);
		const htmlBlob = new Blob([ html ], { type: "text/html" });
		siv3dSetClipboard(htmlBlob, plainTextPtr, plainTextLength);
	},

	siv3dSetClipboardData__deps: ["$siv3dSetClipboard", "$UTF32ToString"],
	siv3dSetClipboardData: (mimeTypePtr, mimeTypeLength, dataPtr, dataSize, plainTextPtr, plainTextLength) => {
		const mimeType = UTF32ToString(mimeTypePtr, mimeTypeLength * 4, false);
		if (!ClipboardItem.supports(mimeType)) {
			return;
		}
		const data = HEAPU8.subarray(dataPtr, dataPtr + dataSize);
		const dataBlob = new Blob([ data ], { type: mimeType });
		siv3dSetClipboard(dataBlob, plainTextPtr, plainTextLength);
	},

	siv3dGetClipboardData__deps: ["$siv3dGetClipboard", "$UTF32ToString"],
	siv3dGetClipboardData__async: "auto",
	siv3dGetClipboardData: async (mimeTypePtr, mimeTypeLength, dataSizePtr) => {
		const mimeType = UTF32ToString(mimeTypePtr, mimeTypeLength * 4, false);
		if (!ClipboardItem.supports(mimeType)) {
			return 0;
		}
		return await siv3dGetClipboard(mimeType, dataSizePtr);
	},

	siv3dGetClipboardAvailableMimeTypes__deps: ["$siv3dUserActivation", "$lengthBytesUTF32", "malloc", "$stringToUTF32"],
	siv3dGetClipboardAvailableMimeTypes__async: "auto",
	siv3dGetClipboardAvailableMimeTypes: async () => {
		await siv3dUserActivation.wait();
		const items = await navigator.clipboard.read();
		if (items.length > 0) {
			const types = items[0].types;
			const pointerListSize = (types.length + 1) * {{{ POINTER_SIZE }}};
			const typesString = types.join("\0");
			const typesStringSize = lengthBytesUTF32(typesString) + 4;
			const ptr = _malloc(pointerListSize + typesStringSize);
			if (ptr) {
				let pointerListPtr = ptr;
				let typesStringPtr = ptr + pointerListSize;
				for (const type of types) {
					{{{ makeSetValue("pointerListPtr", 0, "typesStringPtr", "u32*") }}};
					pointerListPtr += {{{ POINTER_SIZE }}};
					typesStringPtr += lengthBytesUTF32(type) + 4;
				}
				{{{ makeSetValue("pointerListPtr", 0, 0, "u32*") }}};
				stringToUTF32(typesString, ptr + pointerListSize, typesStringSize);
				return ptr;
			}
		}
		return 0;
	},
});
