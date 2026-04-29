Imports System.IO
Imports System.Text.RegularExpressions
Public Class Form1

    Private WithEvents p As Process
    Private totalBatches As Integer = 0
    Private savedReportPath As String = ""
    Private isUpdating As Boolean = False


    Private Sub btnRunReport_Click(sender As Object, e As EventArgs) Handles btnRunReport.Click
        lblStatus.Text = "Running report ..."
        btnRunReport.Enabled = False
        progressBar1.Value = 0

        If Not chkIndex.Checked AndAlso Not chkConstituents.Checked AndAlso Not chkVolatility.Checked AndAlso Not chkMomentum.Checked AndAlso Not chkSector.Checked AndAlso Not chkLiquidity.Checked Then
            MessageBox.Show("Please select at least one report section to generate.")
            btnRunReport.Enabled = True
            Exit Sub
        End If

        StartPythonProcess()
    End Sub

    Private Sub chkLiquidity_CheckedChanged(sender As Object, e As EventArgs) Handles chkLiquidity.CheckedChanged
        If isUpdating Then Exit Sub

        isUpdating = True

        If chkLiquidity.Checked Then
            chkSector.Checked = True
            chkMomentum.Checked = True
            chkVolatility.Checked = True
        End If
        isUpdating = False
    End Sub

    Private Sub chkSector_CheckedChanged(sender As Object, e As EventArgs) Handles chkSector.CheckedChanged
        If isUpdating Then Exit Sub

        isUpdating = True

        If chkSector.Checked Then
            chkVolatility.Checked = True
            chkMomentum.Checked = True
        Else
            chkRisk.Checked = False
            chkLiquidity.Checked = False
        End If
        isUpdating = False

    End Sub

    Private Sub chkVolatility_CheckedChanged(sender As Object, e As EventArgs) Handles chkVolatility.CheckedChanged
        If isUpdating Then Exit Sub

        If Not chkVolatility.Checked AndAlso chkSector.Checked Then

            isUpdating = True
            chkSector.Checked = False
            chkRisk.Checked = False
            chkLiquidity.Checked = False
            isUpdating = False

        End If
    End Sub

    Private Sub chkMomentum_CheckedChanged(sender As Object, e As EventArgs) Handles chkMomentum.CheckedChanged
        If isUpdating Then Exit Sub

        If Not chkMomentum.Checked AndAlso chkSector.Checked Then

            isUpdating = True
            chkSector.Checked = False
            chkRisk.Checked = False
            chkLiquidity.Checked = False
            isUpdating = False

        End If
    End Sub

    Private Sub chkRisk_CheckedChanged(sender As Object, e As EventArgs) Handles chkRisk.CheckedChanged
        If isUpdating Then Exit Sub

        If chkRisk.Checked Then
            isUpdating = True
            chkSector.Checked = True
            chkVolatility.Checked = True
            chkMomentum.Checked = True
            isUpdating = False
        End If
    End Sub
    Private Sub StartPythonProcess()
        p = New Process()
        p.StartInfo.FileName = "D:\VB_learning\scripts\.venv\Scripts\python.exe"
        Dim args As String = """D:\VB_learning\scripts\generate_report.py"""

        If chkIndex.Checked Then args &= " --index"
        If chkConstituents.Checked Then args &= " --constituents"
        If chkVolatility.Checked Then args &= " --volatility"
        If chkMomentum.Checked Then args &= " --momentum"
        If chkSector.Checked Then args &= " --sector"
        If chkRisk.Checked Then args &= " --risk"
        If chkLiquidity.Checked Then args &= " --liquidity"

        p.StartInfo.Arguments = args

        p.StartInfo.UseShellExecute = False
        p.StartInfo.RedirectStandardOutput = True
        p.StartInfo.RedirectStandardError = True
        p.StartInfo.CreateNoWindow = True
        p.EnableRaisingEvents = True

        AddHandler p.OutputDataReceived, AddressOf ProcessOutputHandler
        AddHandler p.ErrorDataReceived, AddressOf ProcessErrorHandler
        AddHandler p.Exited, AddressOf ProcessExitedHandler

        p.Start()
        p.BeginOutputReadLine()
        p.BeginErrorReadLine()
    End Sub

    Private Sub ProcessOutputHandler(sender As Object, e As DataReceivedEventArgs)
        If e.Data Is Nothing Then Return

        Me.Invoke(Sub()
                      Dim line As String = e.Data
                      lblStatus.Text = line

                      If line.Contains("Report saved to:") Then
                          savedReportPath = line.Replace("Report saved to:", "").Trim()
                      End If

                      Dim match = Regex.Match(line, "Downloading batch (\d+) of (\d+)")
                      If match.Success Then
                          Dim batch = Integer.Parse(match.Groups(1).Value)
                          totalBatches = Integer.Parse(match.Groups(2).Value)

                          Dim percent = CInt((batch / totalBatches) * 100)
                          progressBar1.Value = percent
                      End If
                  End Sub)
    End Sub

    Private Sub ProcessErrorHandler(sender As Object, e As DataReceivedEventArgs)
        If e.Data Is Nothing Then Return

        Me.Invoke(Sub()
                      lblStatus.Text = e.Data
                  End Sub)
    End Sub

    Private Sub ProcessExitedHandler(sender As Object, e As EventArgs)
        Me.Invoke(Sub()
                      lblStatus.Text = "Report complete!"
                      btnRunReport.Enabled = True
                      progressBar1.Value = 100


                      If File.Exists(savedReportPath) Then
                          Process.Start(New ProcessStartInfo(savedReportPath) With {.UseShellExecute = True})
                      Else
                          MessageBox.Show("Report created, but file not found." & vbCrLf & savedReportPath)
                      End If
                  End Sub)
    End Sub
End Class

