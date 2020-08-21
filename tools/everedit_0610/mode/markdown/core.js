var App=window.external;
var old_id=-1;
var old_dirty_id=-1;
var el_content = document.getElementById('content');
var highlight_code=true;
var old_doc=null;

function InitMarked()
{
	var renderer = new Markdown.marked.Renderer();
	renderer.listitem = function(text) {
		if (/^ *\[[xX ]\]\s*/.test(text)) {
			text = text
				.replace(/^\s*\[ \] */, '<input type="checkbox" disabled> ')
				.replace(/^\s*\[[xX]\] */, '<input type="checkbox" checked disabled> ');
			return '<li style="list-style: none">' + text + '</li>';
		} else {
			return '<li>' + text + '</li>';
		}
	};

	renderer.code = function(code, language){
		code = code.replace(/</g, '&lt;').replace(/>/g, '&gt;');
		code = code.replace(/\r\n|\r|\n/g, '<br>');//???
		if(language=='' || language==undefined)
			return '<pre class="prettyprint">'+code+'</pre>';
		return '<pre class="prettyprint lang-'+ language +'">'+code+'</pre>';
	};

	Markdown.marked.setOptions({
		renderer: renderer,
		footnotes : true
	});
}

function Preview()
{
	var doc=App.ActiveDoc;
	if( doc==null )
		doc=old_doc;
	if( doc==null )
		return;

	old_doc=doc;
	var dirty_id=doc.DirtyId;
	if( doc.id==old_id && dirty_id==old_dirty_id )
		return;

	if( doc.Syntax.toLowerCase()!="markdown" )
		return;

	var rs = Markdown.marked( doc.Text );
	el_content.innerHTML = rs;

	var tocs=$("*[name='toc']");
	if( tocs.length ){
		var toc=generated_toc.generate('content');
		tocs.each(function(){
			$(this).html( toc );
		});
	}

	old_id=doc.id;
	old_dirty_id=dirty_id;
	prettyPrint();
}

InitMarked();

window.onload = function()
{
	Preview();
	var instant_update=true;
	if( instant_update ){
		setInterval(Preview, 1000);
	}
}
