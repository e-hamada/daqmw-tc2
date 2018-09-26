(テーマ)SampleReader-SampleMonitor間のデータ
================================

この章では、まずSampleReaderおよびSampleMonitorの処理について説明する。
処理を理解した上で、SampleReader-SampleMonitor間のデータを確認する。

DAQ-Middlwareオリジナルのデータ構造があるため、
SampleReader-SampleMonitor間のデータには
エミュレータから受け取ったデータ以外も含まれることに注意すること。


実習内容
--------

シーケンス番号が100で割り切れる時、SampleReaderが送るデータの初めの20Byteを確認する。

また、SampleMonitorが受け取ったそのシーケンス番号のデータを受け取った際、SampleMonitorが送ったデータと一致することを確認する。

