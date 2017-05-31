window.onload = (): void => {
    showDirectory();
};

function showDirectory(): void {
    const directoryRequest: XMLHttpRequest = new XMLHttpRequest();
    directoryRequest.onreadystatechange = (): void => {
        if (4 === directoryRequest.readyState && 200 === directoryRequest.status) {
            const parsed: any = JSON.parse(directoryRequest.responseText);
            const list: HTMLElement = document.getElementById("folders");
            while (list.firstChild) {
                list.removeChild(list.firstChild);
            }
            for (const elem in parsed.elements) {
                if (parsed.elements.hasOwnProperty(elem)) {
                    const li: HTMLElement = document.createElement("li");
                    li.innerHTML = parsed.elements[elem];
                    list.appendChild(li);
                }
            }
        }
    };
    directoryRequest.open("GET", "/files");
    directoryRequest.send();
}

function isOctetStream(contentType: string): boolean {
    return contentType === "Application/octet-stream";
}
function isVideo(contentType: string): boolean {
    return contentType.substring(0, 5) === "Video";
}
function isAudio(contentType: string): boolean {
    return contentType.substring(0, 5) === "Audio";
}
const getButton: HTMLInputElement = document.getElementById("get_button") as HTMLInputElement;

getButton.onclick = (): void => {
    const fileForGet: HTMLInputElement = document.getElementById("file_path") as HTMLInputElement;
    const nameOfFileForGet: string = fileForGet.value;
    const getRequest: XMLHttpRequest = new XMLHttpRequest();
    const re = /(?:\.([^.]+))?$/;
    const extension = re.exec(nameOfFileForGet)[1];
    if (extension !== "txt" && extension !== "html" && extension !== "css" && extension !== "js") {
        getRequest.responseType = "blob";
    }
    getRequest.open("GET", encodeURI(nameOfFileForGet));
    getRequest.send();

    getRequest.onreadystatechange = (): void => {
        if (4 === getRequest.readyState) {
            const infoBlock = document.getElementById("info_block");
            if (200 === getRequest.status) {
                const contentType = getRequest.getResponseHeader("content-type");
                infoBlock.innerHTML = "";
                if (contentType.substring(0, 5) === "Image") {
                    const img: HTMLImageElement = document.createElement("img");
                    img.onload = (): void => {
                        window.URL.revokeObjectURL(img.src);
                    };
                    img.src = window.URL.createObjectURL(getRequest.response);
                    img.className = "response";
                    infoBlock.appendChild(img);
                } else if (isOctetStream(contentType) || isVideo(contentType) || isAudio(contentType)) {
                    const downloadLink = document.createElement("a");
                    downloadLink.setAttribute("href", nameOfFileForGet);
                    downloadLink.setAttribute("download", nameOfFileForGet);
                    infoBlock.appendChild(downloadLink);
                    downloadLink.click();
                    infoBlock.removeChild(downloadLink);
                } else if (contentType.substring(0, 4) === "Text") {
                    infoBlock.innerHTML = getRequest.responseText;
                } else {
                    const directoryRequest: XMLHttpRequest = new XMLHttpRequest();
                    directoryRequest.onreadystatechange = (): void => {
                    if (4 === directoryRequest.readyState && 200 === directoryRequest.status) {
                        const parsed: any = JSON.parse(directoryRequest.responseText);
                        const list: HTMLElement = document.createElement("ul");
                        while (list.firstChild) {
                            list.removeChild(list.firstChild);
                        }
                        for (const elem in parsed.elements) {
                            if (parsed.elements.hasOwnProperty(elem)) {
                                const li: HTMLElement = document.createElement("li");
                                li.innerHTML = parsed.elements[elem];
                                list.appendChild(li);
                            }
                        }
                        list.className = "folders_list";
                        infoBlock.appendChild(list);
                    }
                    };
                    directoryRequest.open("GET", nameOfFileForGet);
                    directoryRequest.send();
                }
            } else {
                infoBlock.innerHTML = getRequest.status + getRequest.statusText;
            }
        }
    };
};

const deleteButton: HTMLInputElement = document.getElementById("delete_button") as HTMLInputElement;

deleteButton.onclick = (): void => {
    const file: HTMLInputElement = document.getElementById("file_path") as HTMLInputElement;
    const fileName: string = file.value;
    const deleteRequest: XMLHttpRequest = new XMLHttpRequest();
    deleteRequest.onreadystatechange = (): void => {
        if (deleteRequest.readyState === 4) {
            if (200 === deleteRequest.status) {
                showDirectory();
            }
            const infoBlock = document.getElementById("info_block");
            infoBlock.innerHTML = deleteRequest.responseText;
        }
    };
    deleteRequest.open("DELETE", encodeURI(fileName));
    deleteRequest.send();
};
