/**
 * JsDoc for EverEdit
 * ptma(ptma@163.com)
 */
(function() {
    var Utils = {
        trim: function(text) {
            return text.replace(/(^\s*)|(\s*$)/g, "");
        },
        getLine: function(line) {
            if (line >= App.ActiveDoc.LineCount) {
                return "";
            }
            return App.ActiveDoc.GetLineText(line);
        },
        write: function(text) {
            App.ActiveDoc.InsertSnippet(text);
        },
        console: function(text) {
            App.OutputWindow.OutputLine(text);
        }
    }

    var jsParser = {
        parseFunction: function(line) {
            var res = /(?:([a-zA-Z_$][a-zA-Z_$0-9\.]*)\s*[:=]\s*)?function(?:\s+([a-zA-Z_$][a-zA-Z_$0-9]*))?\s*\(\s*(.*)\)/;
            var m = res.exec(line);
            if (m) {
                return [(m[1] || m[2]), m[3]];
            }
            return null;
        },
        formatFunction: function(name, args, retval) {
            var out = "";
            out += (name || "") + "$0";
            if (args) {
                // remove comments inside the argument list.
                args = args.replace("/\*.*?\*/", "");
                var parsedArgs = this.parseArgs(args);

                for (var i = 0; i < parsedArgs.length; i++) {
                    out += "\n* @param " + parsedArgs[i] + " ";
                }
            }
            if (retval) {
                out += "\n* @return " + retval;
            }
            out += "\n*/";
            return out;
        },
        parseArgs: function(args) {
            var out = [];

            if (!args) return out;

            // the current token
            current = '';

            // characters which open a section inside which commas are not separators between different arguments
            openQuotes = '"\'<(';
            // characters which close the the section. The position of the character here should match the opening
            // indicator in `openQuotes`
            closeQuotes = '"\'>)';

            matchingQuote = ''
            insideQuotes = false
            nextIsLiteral = false
            blocks = []

            for (var i = 0; i < args.length; i++) {
                var chr = args.charAt(i);
                if (nextIsLiteral) { //# previous char was a \
                    current += chr;
                    nextIsLiteral = false;
                } else if (chr == '\\') {
                    nextIsLiteral = true;
                } else if (insideQuotes) {
                    current += chr;
                    if (chr == matchingQuote) insideQuotes = false;
                } else {
                    if (chr == ',') {
                        blocks.push(Utils.trim(current));
                        current = '';
                    } else {
                        current += chr;
                        quoteIndex = openQuotes.indexOf(chr)
                        if (quoteIndex > -1) {
                            matchingQuote = closeQuotes.charAt(quoteIndex);
                            insideQuotes = true;
                        }
                    }
                }
            }
            blocks.push(Utils.trim(current));
            for (var i = 0; i < blocks.length; i++) {
                out.push(blocks[i]);
            }
            return out
        }
    };

    var javaParser = {
        parseFunction: function(line) {
            var res = /(?:(?:public|protected|private|static|abstract|final|transient|synchronized|native|strictfp)\s+)*([a-zA-Z_$][\<\>\., a-zA-Z_$0-9]+)\s+([a-zA-Z_$][a-zA-Z_$0-9]*)\s*\((.*)\)\s*(?:throws){0,1}\s*([a-zA-Z_$0-9\.,\s]*)/;
            var m = res.exec(line);
            if (m) {
                return [m[2], m[3], m[1], m[4] || ""];
            }
            return null;
        },
        formatFunction: function(name, args, retval, throws) {
            var out = "";
            out += (name || "") + "$0";
            if (args) {
                // remove comments inside the argument list.
                args = args.replace("/\*.*?\*/", "");
                var parsedArgs = this.parseArgs(args);
                for (var i = 0; i < parsedArgs.length; i++) {
                    out += "\n* @param " + parsedArgs[i] + " ";
                }
            }
            if (retval && (retval != "void")) {
                out += "\n* @return " + retval;
            }
            if (throws) {
                throws = throws.replace("/\*.*?\*/", "");
                var parsedThrows = this.parseArgs(throws);
                for (var i = 0; i < parsedThrows.length; i++) {
                    out += "\n* @throws " + parsedThrows[i] + " ";
                }
            }
            out += "\n*/";
            return out;
        },
        parseArgs: function(args) {
            var out = [];

            if (!args) return out;

            // the current token
            current = '';

            // characters which open a section inside which commas are not separators between different arguments
            openQuotes = '"\'<(';
            // characters which close the the section. The position of the character here should match the opening
            // indicator in `openQuotes`
            closeQuotes = '"\'>)';

            matchingQuote = ''
            insideQuotes = false
            nextIsLiteral = false
            blocks = []

            for (var i = 0; i < args.length; i++) {
                var chr = args.charAt(i);
                if (nextIsLiteral) { //# previous char was a \
                    current += chr;
                    nextIsLiteral = false;
                } else if (chr == '\\') {
                    nextIsLiteral = true;
                } else if (insideQuotes) {
                    current += chr;
                    if (chr == matchingQuote) insideQuotes = false;
                } else {
                    if (chr == ',') {
                        blocks.push(Utils.trim(current));
                        current = '';
                    } else {
                        current += chr;
                        quoteIndex = openQuotes.indexOf(chr)
                        if (quoteIndex > -1) {
                            matchingQuote = closeQuotes.charAt(quoteIndex);
                            insideQuotes = true;
                        }
                    }
                }
            }
            blocks.push(Utils.trim(current));
            for (var i = 0; i < blocks.length; i++) {
                out.push(blocks[i]);
            }
            return out
        }
    };
    var JsDoc = {
        currentLine: "",
        fullCurrentLine: "",
        nextLine: "",
        parser: jsParser,
        
        inDocComment: function(line) {
            return /^\s*\/\*\*/.test(line) || /^\s*\*/.test(line);
        },
        isDocCommentEnd: function(line) {
            return /^\s*\*\//.test(line);
        },
        isDocCommentStart: function(line) {
            return /^\s*\/\*\*/.test(line);
        },
        isExistingComment: function(line) {
            return /^\s*\*/.test(line);
        },

        initialize: function() {
            var LineText = Utils.getLine(App.ActiveDoc.CaretLine);
            this.fullCurrentLine = LineText;
            this.currentLine = LineText.substring(0, App.ActiveDoc.CaretCol);
            this.nextLine = Utils.getLine(App.ActiveDoc.CaretLine + 1);
        },
        run: function() {
            var syntax = App.ActiveDoc.Syntax;
            if (syntax != "JavaScript" && syntax != "Java") {
                Utils.write("\n");
                return;
            }
            this.initialize();
            if (!this.inDocComment(this.currentLine)) {
                Utils.write("\n");
                return;
            }
            var nextLine = this.nextLine;
            if (this.isDocCommentEnd(this.currentLine)) {
                Utils.write("\n");
                return;
            }
            if (this.inDocComment(this.currentLine)) {
                if (this.isExistingComment(nextLine)) {
                    if (this.isDocCommentStart(this.currentLine)) {
                        Utils.write("\n * ");
                    } else {
                        Utils.write("\n* ");
                    }
                    return;
                }
                if (this.isDocCommentEnd(this.fullCurrentLine)) {
                    Utils.write("\n*");
                    return;
                }
            }

            if (syntax == "Java") {
                this.parser = javaParser;
            }
            var out = this.parser.parseFunction.call(this.parser, nextLine);
            if (out) {
                out = this.parser.formatFunction.apply(this.parser, out);
                if (this.isDocCommentStart(this.currentLine)) {
                    Utils.write("\n * ");
                } else if (!this.isDocCommentEnd(this.currentLine)) {
                    Utils.write("\n*");
                }
                Utils.write(out);
            } else {
                Utils.write("\n * $0\n */");
            }
        }
    };
    JsDoc.run();
})();