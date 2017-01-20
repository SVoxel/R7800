/*--------------------------------------------------|
| dTree 2.05 | www.destroydrop.com/javascript/tree/ |
|---------------------------------------------------|
| Copyright (c) 2002-2003 Geir Landrö               |
|                                                   |
| This script can be used freely as long as all     |
| copyright messages are intact.                    |
|                                                   |
| Updated: 17.04.2003                               |
|--------------------------------------------------*/

// Node object
function Node(id, pid, name, io, shc, icon, iconOpen, open) {
	this.id = id;
	this.pid = pid;
	this.name = name;
	this.io = io;
	this.shc = shc;
	this.icon = icon;
	this.iconOpen = iconOpen;
	this._is = false;
	this._ls = false;
	this._hc = false;
	this._ai = 0;
	this._p;
};

// Tree object
function dTree(objName,selectedNode) {

	this.icon = {
		root				: 'base.gif',
		folder			: 'folder.gif',
		folderOpen	: 'folderopen.gif',
		node				: 'page.gif',
		empty				: 'empty.gif',
		line				: 'line.gif',
		join				: 'join.gif',
		joinBottom	: 'joinbottom.gif',
		plus				: 'plus.gif',
		plusBottom	: 'plusbottom.gif',
		minus				: 'minus.gif',
		minusBottom	: 'minusbottom.gif'
	};
	this.obj = objName;
	this.aNodes = [];
	this.aIndent = [];
	this.root = new Node(-1);
	this.selectedNode = selectedNode;
	this.completed = false;
};

// Adds a new node to the node array
dTree.prototype.add = function(id, pid, name, io, shc, icon, iconOpen, open) {
	this.aNodes[this.aNodes.length] = new Node(id, pid, name, io, shc, icon, iconOpen, open);
};

// Outputs the tree to the page
dTree.prototype.toString = function() {
	var str = '<div class="dtree">\n';
	if (document.getElementById) {
		str += this.addNode(this.root);
	} else str += 'Browser not supported.';
	str += '</div>';
	this.completed = true;
	return str;
};

// Creates the tree structure
dTree.prototype.addNode = function(pNode) {
	var str = '';
	var n=0;
	for (n; n<this.aNodes.length; n++) {
		if (this.aNodes[n].pid == pNode.id) {
			var cn = this.aNodes[n];
			cn._p = pNode;
			cn._ai = n;
			this.setCS(cn);

			if ( cn.id == this.selectedNode ) {
					cn._is = true;
					this.selectedNode = n;
			}

			str += this.node(cn, n);
			if (cn._ls) break;
		}
	}
	return str;
};

// Creates the node icon, url and text
dTree.prototype.node = function(node, nodeId) {
	var str = '<div class="dTreeNode">' + this.indent(node, nodeId);

		if (!node.icon) node.icon = (this.root.id == node.pid) ? this.icon.root : this.icon.folder;
		if (!node.iconOpen) node.iconOpen =  this.icon.folderOpen ;
		if (this.root.id == node.pid) {
			node.icon = this.icon.root;
			node.iconOpen = this.icon.root;
		}
		str += '<img id="i' + this.obj + nodeId + '" src="' + ((node.io == '1') ? node.iconOpen : node.icon) + '" alt="" />';
		
		if(this.root.id == node.pid){
		str += '<a id="s' + this.obj + nodeId + '">';
		}
		else{
		str += '<a id="s' + this.obj + nodeId + '" href="javascript: ' + this.obj + '.s(' + nodeId + ');"  class="'+((node.id == this.selectedNode) ? 'nodeSel' : 'node')+'">';
		}
	str += node.name;

	str += '</a>';
	str += '</div>';
	if (node._hc) {
		str += '<div id="d' + this.obj + nodeId + '" class="clip" style="display:' + ((this.root.id == node.pid || node.io == '1' ) ? 'block' : 'none') + ';">';
		str += this.addNode(node);
		str += '</div>';
	}
	else
		str += '<div id="d' + this.obj + nodeId + '" class="clip"></div>';
	this.aIndent.pop();
	return str;
};
function test()
{
//	alert("test");
}
// Adds the empty and line icons
dTree.prototype.indent = function(node, nodeId) {
	var str = '';
	if (this.root.id != node.pid) {
		for (var n=0; n<this.aIndent.length; n++)
			str += '<img src="' + ( (this.aIndent[n] == 1) ? this.icon.line : this.icon.empty ) + '" alt="" />';
		(node._ls) ? this.aIndent.push(0) : this.aIndent.push(1);
		if (node.shc == '1') {
			str += '<a href="javascript: ' + this.obj + '.s(' + nodeId + ');"><img id="j' + this.obj + nodeId + '" src="';

		 str += ( (node.io == '1') ? ((node._ls ) ? this.icon.minusBottom : this.icon.minus) : ((node._ls ) ? this.icon.plusBottom : this.icon.plus ) );
			str += '" alt="" /></a>';

		} else str += '<img id="j' + this.obj + nodeId + '" src="' + ((node._ls) ? this.icon.joinBottom : this.icon.join ) + '" alt="" />';
	}
	return str;
};

// Checks if a node has any children and if it is the last sibling
dTree.prototype.setCS = function(node) {
	var lastId;
	for (var n=0; n<this.aNodes.length; n++) {
		if (this.aNodes[n].pid == node.id) node._hc = true;
		if (this.aNodes[n].pid == node.pid) lastId = this.aNodes[n].id;
	}
	if (lastId==node.id) node._ls = true;
};

// Highlights the selected node
dTree.prototype.s = function(id) {

	var cn = this.aNodes[id];
//.o
	var Dio=(cn.io == '1')? '0':'1';
	cn.io = Dio;

	if (this.selectedNode != id) 
		if (this.selectedNode || this.selectedNode==0) {
			eOld = document.getElementById("s" + this.obj + this.selectedNode);
			eOld.className = "node";
		}
	
		eNew = document.getElementById("s" + this.obj + id);
		eNew.className = "nodeSel";
		this.selectedNode = id;
	
	var form=document.forms[0];
	var str = eval('node' + id).replace(/&nbsp;/g," ").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
	echo_info = str.split('*');
	form.nodenumber.value=id;
	if(echo_info[0]=='0')// volume name could have '*', volume name save in nodevol, so disk node's value less a item than normal notes.
	{
		form.currentpath.value=echo_info[2];
		parent.bottom.document.forms[0].folder_path.value=echo_info[2];
		var value=""+echo_info[0]+"*"+cn.io+"*"+echo_info[2]+"*"+echo_info[3]+"*"+echo_info[4];
	}
	else
	{
		form.currentpath.value=echo_info[3];
		parent.bottom.document.forms[0].folder_path.value=echo_info[3];
		var value=""+echo_info[0]+"*"+echo_info[1]+"*"+cn.io+"*"+echo_info[3]+"*"+echo_info[4]+"*"+echo_info[5];
	}

	form.nodevalue.value=value;

	if( echo_info[5] == "folder" || (echo_info[0]=='0' && echo_info[4] == "folder"))
	{
		if (cn.io == '1'){
			open_browser(form);
		}
		else{
			close_browser(form);
		}
	}
}
// If Push and pop is not implemented by the browser
if (!Array.prototype.push) {
	Array.prototype.push = function array_push() {
		for(var i=0;i<arguments.length;i++)
			this[this.length]=arguments[i];
		return this.length;
	}
};
if (!Array.prototype.pop) {
	Array.prototype.pop = function array_pop() {
		lastElement = this[this.length-1];
		this.length = Math.max(this.length-1,0);
		return lastElement;
	}
};
