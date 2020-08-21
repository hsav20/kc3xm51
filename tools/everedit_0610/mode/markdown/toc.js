generated_toc = {
	generate: function(content_id) {
		generate_from = '0';
		generate_for = 'unset';
		tocparent = document.createElement('div');
		top_node = document.getElementById(content_id);
		if (generate_from == 0) {
			first_header_found = generated_toc.findFirstHeader(top_node);
			if (!first_header_found) {
				return '';
			} else {
				generate_from = first_header_found.toLowerCase().substr(1);
			}
		}
		headings_to_treat = {
			"h6": ''
		};
		for (var i = 5; i >= parseInt(generate_from); i--) {
			headings_to_treat["h" + i] = '';
		}

		nodes = top_node.all ? top_node.all : top_node.getElementsByTagName('*');
		headings = [];
		for (var i = 0; i < nodes.length; i++) {
			if (nodes[i].nodeName.toLowerCase() in headings_to_treat) {
				if ((' ' + nodes[i].className + ' ').indexOf('no-TOC') != -1) {
					continue;
				}
				headings.push(nodes[i]);
			}
		}

		cur_head_lvl = "h" + generate_from;
		cur_list_el = document.createElement('ul');
		p = document.createElement('p');
		span = document.createElement('span');
		span.className = 'hidden';

		p.appendChild(span);
		tocparent.appendChild(p);
		tocparent.appendChild(cur_list_el);

		// now walk through our saved heading nodes
		for (var i = 0; i < headings.length; i++) {
			this_head_el = headings[i];
			this_head_lvl = headings[i].nodeName.toLowerCase();
			if (!this_head_el.id) {
				// if heading doesn't have an ID, give it one
				this_head_el.id = 'heading_toc_j_' + i;
				this_head_el.setAttribute('tabindex', '-1');
			}

			while (this_head_lvl > cur_head_lvl) {
				var last_listitem_el = null;
				for (var j = 0; j < cur_list_el.childNodes.length; j++) {
					if (cur_list_el.childNodes[j].nodeName.toLowerCase() == 'li') {
						last_listitem_el = cur_list_el.childNodes[j];
					}
				}
				if (!last_listitem_el) {
					// there aren't any LIs, so create a new one to add the UL to
					last_listitem_el = document.createElement('li');
				}
				new_list_el = document.createElement('ul');
				last_listitem_el.appendChild(new_list_el);
				cur_list_el.appendChild(last_listitem_el);
				cur_list_el = new_list_el;
				cur_head_lvl = 'h' + (parseInt(cur_head_lvl.substr(1, 1)) + 1);
			}

			while (this_head_lvl < cur_head_lvl) {
				// this heading is at a higher level than the last one;
				// go back up the TOC to put it at the right level
				cur_list_el = cur_list_el.parentNode.parentNode;
				cur_head_lvl = 'h' + (parseInt(cur_head_lvl.substr(1, 1)) - 1);
			}

			// create a link to this heading, and add it to the TOC
			li = document.createElement('li');
			a = document.createElement('a');
			a.href = '#' + this_head_el.id;
			a.appendChild(document.createTextNode(generated_toc.innerText(this_head_el)));
			li.appendChild(a);
			cur_list_el.appendChild(li);
		}

		// add an aftertoc paragraph as destination for the skip-toc link
		p = document.createElement('p');
		p.id = 'aftertoc';
		tocparent.appendChild(p);

		// go through the TOC and find all LIs that are "empty", i.e., contain
		// only ULs and no links, and give them class="missing"
		var alllis = tocparent.getElementsByTagName("li");
		for (var i = 0; i < alllis.length; i++) {
			var foundlink = false;
			for (var j = 0; j < alllis[i].childNodes.length; j++) {
				if (alllis[i].childNodes[j].nodeName.toLowerCase() == 'a') {
					foundlink = true;
				}
			}
			if (!foundlink) {
				alllis[i].className = "missing";
			} else {
				alllis[i].className = "notmissing";
			}
		}

		return tocparent.innerHTML;
	},

	innerText: function(el) {
		return (typeof(el.innerText) != 'undefined') ? el.innerText :
			(typeof(el.textContent) != 'undefined') ? el.textContent :
			el.innerHTML.replace(/<[^>]+>/g, '');
	},

	findFirstHeader: function(node) {
		// a recursive function which returns the first header it finds inside
		// node, or null if there are no functions inside node.
		var nn = node.nodeName.toLowerCase();
		if (nn.match(/^h[1-6]$/)) {
			// this node is itself a header; return our name
			return nn;
		} else {
			for (var i = 0; i < node.childNodes.length; i++) {
				var subvalue = generated_toc.findFirstHeader(node.childNodes[i]);
				// if one of the subnodes finds a header, abort the loop and return it
				if (subvalue) return subvalue;
			}
			// no headers in this node at all
			return null;
		}
	}
};