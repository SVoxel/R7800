(function ($$) {

        "use strict";


        $$(function () {

		$$.PAGE_WAITING_DIV ="<div id=\"page_waiting\" class=\"running\"></div><div class=\"page_doing running\"> \
                          <div class=\"loadingMessage roundCorners\">请等待一会...</div></div>"

		$$.submit_wait = function(container, string) {
                        $$(container).append(string);
                };
		/**
                 * @function post form data.
                 * @param {container id or class} - the id or class of the container
                 * @param {url} - request url, if it's NULL or empty, it will auto get formId's action value.
                 * @param {function} - callback function
                 */
                $$.postForm = function (container, url, callback) {
			url = $$(container).attr("action");
                        var funcs = function(json) {
                                if ( callback != null )
                                        callback(json);
                        };

                        $$.ajax({
                                url: url,
                                type: "POST",
                                data: $$(container).serialize(),
                                dataType: 'json',
                                contentType: "application/json; charset=utf-8",
                                success: funcs
                        });
		};

		$$.getData = function (url, callback, ajax_error) {
			var new_url = url;

			$$.ajax({
				url: new_url,
				success: function(json) {
					callback(json);
				},
				dataType: "json",
				error: function(){
					ajax_error();
				}
			});
		};

		/*******************************************************************************************
                 *
                 * Rewrite alert and confirm dialog
                 *
                 *******************************************************************************************/
                $$.alertBox = function(msg, str, act) {
                        if ( !$$('#modalOverlay').length ) {
                                $$('body').append("<div id='modalOverlay'></div>");
                        }
                        $$('#modalOverlay').addClass('active');
                        $$('#modalOverlay').fadeIn('fast');

                        var date = new Date();
                        var id = "alert"+date.getTime();
                        var close = function() {
                                if ( $$('.modalBox:visible').length == 1 ){
                                        $$('#modalOverlay').fadeOut();
                                }
                                $$('#'+id).fadeOut();
                                $$('#'+id).remove();
                        },
                        action = close;

                        if ( !str ) str = "我知道了";
                        if ( typeof(act) == "function" ) {
                                action = function() {
                                        act();
                                        close();
                                }
                        }

			var data = "<div id='"+id+"'class='modalBox'><div><img id='alertImage' src='/image/pay_issue.png'></div><div id='alertMsg'>温馨提示</div><div class='recommendation'><p>"
                                + msg + "</p><div class='centerButtons'><div class='boxButtons'><a class='alertClose' id='"+id+"Ok'>"
                                + str + "</a></div></div></div></div>";
                        $$('body').append(data);
                        $$('#'+id+'Ok').click(action);
                        //fixed_modal();
                };

	var check_url;
	var count=1;

	function checkWechatCode(check_url) {
		$$.getData(check_url,function(json){
			if(json.code == "1000"){
				$$('.wechat_code').addClass("wechat_success");
				$$('img:first', '.wechat_code').attr('src','/image/funjsq2.png');
				$$('.wechat_success_content').css('display','block');
				$$('.wechat_start_content').css('display','none');
				$$.getData("update_login.aspx", function(json) {
					return;
				});
			}else if(json.code == "1001"){
				$$('.wechat_code').removeClass("wechat_success");
				if(count < 45){
					setTimeout(function(){
						checkWechatCode(check_url);
					}, 2 * 1000);
					count++;
				}else{
					$$('.wechat_code').removeClass("wechat_success");
					$$('.wechat_code').addClass("wechat_invalid");		
					$$('.invalid_alert').css('display','block');
					$$('img:first', '.wechat_code').attr('src','/image/funjsq4.png');
				}
			}else if(json.code == "1002"){
				$$('.wechat_code').removeClass("wechat_success");
				$$('.wechat_code').addClass("wechat_invalid");
				$$('.invalid_alert').css('display','block');
				$$alertBox(json.msg);
			}
		});
	}

	if ( $$('#wechatForm').length ) {
		$$('.wechat_success_content').css('display','none');
		$$('.invalid_alert').css('display','none');
		if(funjsq_no_need_login == "1"){
			$$('img:first', '.wechat_code').attr('src','image/funjsq2.png');
			$$('.wechat_code').addClass("wechat_success");
			$$('.wechat_success_content').css('display','block');
			$$('.wechat_start_content').css('display','none');
		}else{
			$$.getData('https://wxapi.funjsq.com/index.php/wxFunjsq/api/v1/get_qrcode_url',function(json){
				var dec_url = "",dec_ticket="";
				if(json.code == "1000")
				{
					dec_url=decodeURIComponent(json.url);
					dec_ticket=decodeURIComponent(json.ticket);
					$$('img:first', '.wechat_code').attr('src',dec_url);
					$$('.wechat_code').removeClass("wechat_success");
					$$('.wechat_success_contnet').css('display','none');
					$$('.wechat_start_contnet').css('display','block');
					var check_qrcode='https://wxapi.funjsq.com/index.php/wxFunjsq/api/v1/check_qrcode_scan_action?ticket='+dec_ticket+'&R='+module_name+'&MAC='+router_mac;
					checkWechatCode(check_qrcode);
					setTimeout(function(){
					}, 90 * 1000);
				}
			});
		}

		$$('#wechat_bt').click(function() {
			location.href="funjsq_wechat.htm"
		});

	}


}); // end ready function

}(jQuery));
