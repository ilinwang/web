// Global constants
var replacementChar = /%s/
var timeoutID
var playing

// Global variables
var outputTitle
var outputSubTitle
var outputURL
var imagesOnAPage
var index

function parseCommandLine() {
	if (location.href.indexOf("?") == -1) {
		return 0
	}

	// Parse any command line arguments
	urlQuery = location.href.split("?")

	urlTerms = urlQuery[1].split(",")
	if (urlTerms[0] != null) {
		index = parseInt(urlTerms[0])
	}

	if (urlTerms[1] != null) {
		playing = parseInt(urlTerms[1])
	}
}

function printBodyStartTag() {
	document.write("<BODY BGCOLOR=\"" + BGCOLOR + "\" TEXT=\"" + TEXT + "\" LINK=\"" + LINK + "\" VLINK=\"" + VLINK + "\" ALINK=\"" + ALINK + "\">")
   document.write("<STYLE TYPE=\"text/css\">")
   
	document.write(".pageFont")
	document.write("{")
	document.write(pageFontFamily)
	document.write(pageFontSize)
	document.write("}")
	
   document.write(".thumbFont")
	document.write("{")
	document.write(thumbFontFamily)
	document.write(thumbFontSize)
	document.write(thumbFontColor)
	document.write("}")
	
	document.write(".imageFont")
	document.write("{")
	document.write(imageFontFamily)
	document.write(imageFontSize)
	document.write(imageFontColor)
	document.write("}")
	
	document.write("</STYLE>")
}

function stopStartSlideShow(firstTime) {
	if (playing == 1) {
		playing = 0
		clearTimeout(timeoutID)
		
		if (firstTime != 1)
		{
			index = -1
			slideShow()
		}
	}
	else {
		playing = 1
		timeoutID = setInterval("slideShow()", 3000)
	}
}

function slideShow() {
	index = index + 1
	if (index >= imageDB.length) {
		index = 0
	}

	// Change the image
	window.location.href = "page.htm?" + index + "," + playing
	

	// Change the play text to stop
	slideShowControl.innerText="Stop";
}

function init() {
	// Make output strings from theme.js and data.js
    outputTitle = themeTitle.replace(replacementChar, title);
	outputSubTitle = themeSubTitle.replace(replacementChar, subTitle);
	outputURL = themeURL.replace(replacementChar, URL);
    outputURL = outputURL.replace(replacementChar, URL);

	// Perform some initial calculations
	imagesOnAPage = rows*columns
	
	playing = 0
	index = 0
	parseCommandLine()

	// Start or stop the slideshow
	if (playing == 0) {
		playing = 1
	}
	else {
		playing = 0
	}
	stopStartSlideShow(1)
}

function printTitle(writePageTitle) {
   if ((rows == 1 || columns == 1) && writePageTitle == 0) {
      return
   }
	document.write("<DIV CLASS=\"title_div\">")
	document.write("<SPAN CLASS=\"pageFont\">")
	document.write(outputTitle)
	document.write("</SPAN>")
	document.write("</DIV>")
}

function printSubTitle(writePageSubTitle) {
   if ((rows == 1 || columns == 1) && writePageSubTitle == 0) {
      return
   }
	document.write("<DIV CLASS=\"subtitle_div\">")
	document.write("<SPAN CLASS=\"pageFont\">")
	document.write(outputSubTitle)
	document.write("</SPAN>")
	document.write("</DIV>")
}

function printImage() {
	document.write("<DIV CLASS=\"image_div\">")
	document.write("<TABLE>")
	document.write("<TD>")
	document.write("<TR valign=middle>")
	document.write("<TD align=center>")
	outputImageLink = themeImageLink.replace(replacementChar, index);
	outputImageLink = outputImageLink.replace(replacementChar, playing);
	document.write(outputImageLink)
	document.write("</TD>")
	document.write("</TR>")
   for (var i = index*6; i < index*6+6; i++) {
	   document.write("<TR valign=middle>")
	   document.write("<TD CLASS=\"imageFont\" align=center>" + imageMetadataDB[i] + "</TD>")
	   document.write("</TR>")
   }
	document.write("</TD>")
	document.write("</TABLE>");
	document.write("</DIV>")
}

function writeLinks(writePageLinks) {
   if ((rows == 1 || columns == 1) && writeLinks == 1) {
      return
   }
   
	var themePreviousHolder = themeImageLinkPrevious
	var themeNextHolder = themeImageLinkNext
	var themePlayHolder = themeImageLinkPlay
	var themeStopHolder = themeImageLinkStop
	if (writePageLinks == 1)
	{
		themePreviousHolder = themeThumbLinkPrevious
		themeNextHolder = themeThumbLinkNext
		themePlayHolder = themeThumbLinkPlay
		themeStopHolder = themeThumbLinkStop
	}

	document.write("<DIV CLASS=\"links_div\">")

	document.write("<TABLE>")
	document.write("<TR>")

	// Write previous link
	document.write("<TD CLASS=\"pageFont\">")
	document.write("<DIV CLASS=\"previous_div\">")
	if (index != 0) {
		var previousIndex
		if (writePageLinks == 0) {
			previousIndex = index - 1;
			outputPreviousLink = themePreviousHolder.replace(replacementChar, previousIndex);
			outputPreviousLink = outputPreviousLink.replace(replacementChar, playing);
			document.write(outputPreviousLink)
		}
		else {
			if (index != 0) {
				previousIndex = index-imagesOnAPage;
				if (previousIndex < 0) {
					previousIndex = 0	  		
				}
				outputPreviousLink = themePreviousHolder.replace(replacementChar, previousIndex);
				outputPreviousLink = outputPreviousLink.replace(replacementChar, playing);
				document.write(outputPreviousLink)
			}
		}
	}
	document.write("</DIV>")
	document.write("</TD>")
	
	// Write URL
	document.write("<TD CLASS=\"pageFont\">")
	document.write("<DIV CLASS=\"url_div\">")
	document.write(outputURL)
	document.write("</DIV>")
	document.write("</TD>")

	// Write next link
	document.write("<TD CLASS=\"pageFont\">")
	document.write("<DIV CLASS=\"next_div\">")
	var nextIndex
	if (writePageLinks == 0)
	{
		nextIndex = index + 1;
		if (nextIndex < imageDB.length) {
			outputNextLink = themeNextHolder.replace(replacementChar, nextIndex);
			outputNextLink = outputNextLink.replace(replacementChar, playing);
			document.write(outputNextLink)
		}
	}
	else {
	    nextIndex = index+imagesOnAPage;
		if (nextIndex < thumbDB.length) {
			outputNextLink = themeNextHolder.replace(replacementChar, nextIndex);
			outputNextLink = outputNextLink.replace(replacementChar, playing);
			document.write(outputNextLink)
		}
	}
	document.write("</DIV>")
	document.write("</TD>")

	// Write play/stop
	document.write("<TD CLASS=\"pageFont\">")
	document.write("<DIV CLASS=\"slideShowControl_div\">")
	if (playing == 1) {
		document.write(themeStopHolder)
	}
	else {
		document.write(themePlayHolder)
	}
	document.write("</DIV>")
	document.write("</TD>")

	document.write("</TR>")
	document.write("</TABLE>")

	document.write("</DIV>")
}

function printTable() {
	document.write("<DIV CLASS=\"images_div\">")
	document.write("<TABLE>")
	counter = index
	for (var i = 0; i < rows; i++) {
		document.write("<TR>")
		for (var j = 0; j < columns; j++) {
			if (counter < thumbDB.length) {
				document.write("<TD>")
				document.write("<TABLE>")
				document.write("<TD valign=top>")
				document.write("<TR>")
				document.write("<TD align=center>")
				outputThumbLink = themeThumbLink.replace(replacementChar, counter)
				outputThumbLink = outputThumbLink.replace(replacementChar, playing)
				document.write(outputThumbLink)
				document.write("</TD>")
				document.write("</TR>")
				
				// Write out the metadata
				for (var k = counter*6; k < counter*6+6; k++) {
				   document.write("<TR valign=middle>")
				   document.write("<TD CLASS=\"thumbFont\" align=center>" + thumbMetadataDB[k] + "</TD>")
				   document.write("</TR>")
				}
				document.write("</TD>")
				
				document.write("</TABLE>");
				document.write("</TD>")
			}

			counter++
		}
		document.write("</TR>")
	}
	document.write("</TABLE>")
	document.write("</DIV>")
}

function loadImages() {
	for (var i = 0; i < imagesOnAPage; i++) {
		if ((i+index) < thumbDB.length) {
			var j = 0
			while (document.images[j] != null && document.images[j].name == "IGNORE") {
				j++
			}
			if (document.images[i+j] != null) {
				document.images[i+j].src = thumbDB[i+index]
			}
		}
	}
}

function loadImage() {
   var i = 0
   while (document.images[i].name == "IGNORE") {
      i++
   }
	document.images[i].src = imageDB[index]
}
 