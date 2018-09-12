def response(flow):
	if "/kcsapi/" in flow.request.pretty_url:
		print("[[[QU:" + flow.request.pretty_url + "]]]" + "[[[QC:" + flow.request.content.decode("utf-8") + "]]]" + "[[[SC:" + flow.response.content.decode("utf-8") + "]]]");