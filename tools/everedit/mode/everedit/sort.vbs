' A helper function to sort Buttons/Menus
' Copyright@www.Everedit.net
' 2013-04-19

Function ExecuteSort(strSortWhat)
	Dim objRxp
	Set objRxp=New RegExp

	objRxp.Pattern="^\["+strSortWhat+"(\d+)\]$"
	objRxp.IgnoreCase=True
	objRxp.Global=False
	objRxp.MultiLine=True

	'first step: replace [Buttonxx] to [@@btnzjlpxx]
	tmpName="@@btnzjlp"
	
	bRet=False
	For i=0 To 100
		If objRxp.Test(strContent) Then
			strContent=objRxp.Replace( strContent, "["+tmpName+CStr(i)+"]" )
			bRet=True
		Else
			Exit For
		End If
	Next

	'second step: replace [@@btnzjlpxx] to [Buttonxx]
	If bRet=True Then
		objRxp.Global=True
		objRxp.Pattern="^\["+tmpName+"(\d+)\]$"
		strContent=objRxp.Replace( strContent, "["+strSortWhat+"$1]" )
	End If

	ExecuteSort=bRet
End Function