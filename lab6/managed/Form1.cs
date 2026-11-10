namespace Lab6;

public class Form1 : Form
{
    private readonly F1GarageWrapper garage = new();
    private readonly ListBox listCars = new();
    private readonly TextBox txtTeam = new();
    private readonly TextBox txtDriver = new();
    private readonly NumericUpDown numYear = new() { Minimum = 1950, Maximum = 2100, Value = 2024 };
    private readonly NumericUpDown numPower = new() { Minimum = 0, Maximum = 2000, Value = 1000 };
    private readonly NumericUpDown numWeight = new() { Minimum = 100, Maximum = 2000, DecimalPlaces = 1, Value = 800 };
    private readonly CheckBox chkExtended = new() { Text = "Есть данные о команде" };
    private readonly TextBox txtPrincipal = new();
    private readonly NumericUpDown numBudget = new() { Minimum = 0, Maximum = 1000000000, DecimalPlaces = 2, Increment = 100000, Value = 1000000 };
    private readonly TextBox txtSponsors = new() { PlaceholderText = "через запятую" };
    private readonly Label lblStatus = new();

    public Form1()
    {
        Text = "F1 Garage - ЛР6";
        Width = 1000;
        Height = 600;
        StartPosition = FormStartPosition.CenterScreen;
        FormClosed += (_, _) => garage.Dispose();

        var split = new SplitContainer { Dock = DockStyle.Fill, SplitterDistance = 350 };
        Controls.Add(split);

        listCars.Dock = DockStyle.Fill;
        listCars.SelectedIndexChanged += (_, _) => ShowSelected();
        split.Panel1.Controls.Add(listCars);

        var fields = new TableLayoutPanel { Dock = DockStyle.Fill, Padding = new Padding(12), ColumnCount = 2, RowCount = 11 };
        fields.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 130));
        fields.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100));
        split.Panel2.Controls.Add(fields);

        AddRow(fields, 0, "Команда", txtTeam);
        AddRow(fields, 1, "Пилот", txtDriver);
        AddRow(fields, 2, "Год", numYear);
        AddRow(fields, 3, "Мощность, л.с.", numPower);
        AddRow(fields, 4, "Вес, кг", numWeight);
        AddRow(fields, 5, "Тип", chkExtended);
        AddRow(fields, 6, "Руководитель", txtPrincipal);
        AddRow(fields, 7, "Бюджет", numBudget);
        AddRow(fields, 8, "Спонсоры", txtSponsors);

        chkExtended.CheckedChanged += (_, _) => UpdateExtendedFields();
        UpdateExtendedFields();

        var buttons = new FlowLayoutPanel { Dock = DockStyle.Fill, AutoSize = true };
        buttons.Controls.Add(MakeButton("Добавить", AddCar));
        buttons.Controls.Add(MakeButton("Изменить", UpdateSelectedCar));
        buttons.Controls.Add(MakeButton("Удалить", DeleteSelectedCar));
        buttons.Controls.Add(MakeButton("Очистить", () => { garage.Clear(); RefreshList(); }));
        buttons.Controls.Add(MakeButton("Сохранить", SaveGarage));
        buttons.Controls.Add(MakeButton("Загрузить", LoadGarage));
        fields.Controls.Add(buttons, 0, 9);
        fields.SetColumnSpan(buttons, 2);

        lblStatus.Dock = DockStyle.Fill;
        fields.Controls.Add(lblStatus, 0, 10);
        fields.SetColumnSpan(lblStatus, 2);

        RefreshList();
    }

    private static void AddRow(TableLayoutPanel panel, int row, string label, Control control)
    {
        panel.Controls.Add(new Label { Text = label, AutoSize = true, Anchor = AnchorStyles.Left, Margin = new Padding(3, 8, 3, 3) }, 0, row);
        control.Dock = DockStyle.Fill;
        panel.Controls.Add(control, 1, row);
    }

    private static Button MakeButton(string text, Action onClick)
    {
        var button = new Button { Text = text, AutoSize = true, Margin = new Padding(4) };
        button.Click += (_, _) => onClick();
        return button;
    }

    private void UpdateExtendedFields()
    {
        txtPrincipal.Enabled = chkExtended.Checked;
        numBudget.Enabled = chkExtended.Checked;
        txtSponsors.Enabled = chkExtended.Checked;
    }

    private CarInfo ReadFields() => new()
    {
        Team = txtTeam.Text.Trim(),
        Driver = txtDriver.Text.Trim(),
        Year = (int)numYear.Value,
        Power = (int)numPower.Value,
        Weight = (double)numWeight.Value,
        IsExtended = chkExtended.Checked,
        Principal = chkExtended.Checked ? txtPrincipal.Text.Trim() : "",
        Budget = chkExtended.Checked ? (double)numBudget.Value : 0,
        Sponsors = chkExtended.Checked ? txtSponsors.Text.Trim() : ""
    };

    private bool FieldsAreValid()
    {
        if (txtTeam.Text.Trim().Length > 0 && txtDriver.Text.Trim().Length > 0) return true;
        MessageBox.Show("Заполните команду и пилота.");
        return false;
    }

    private void AddCar()
    {
        if (!FieldsAreValid()) return;
        garage.AddCar(ReadFields());
        RefreshList();
        listCars.SelectedIndex = listCars.Items.Count - 1;
    }

    private void UpdateSelectedCar()
    {
        int index = listCars.SelectedIndex;
        if (index < 0) { MessageBox.Show("Выберите машину в списке."); return; }
        if (!FieldsAreValid()) return;
        garage.UpdateCar(index, ReadFields());
        RefreshList();
        listCars.SelectedIndex = index;
    }

    private void DeleteSelectedCar()
    {
        if (listCars.SelectedIndex < 0) return;
        garage.DeleteCar(listCars.SelectedIndex);
        RefreshList();
    }

    private void SaveGarage()
    {
        using var dlg = new SaveFileDialog { Filter = "Архив гаража (*.txt)|*.txt|Все файлы (*.*)|*.*", FileName = "f1garage.txt" };
        if (dlg.ShowDialog() == DialogResult.OK)
            lblStatus.Text = garage.Save(dlg.FileName) ? "Данные сохранены" : "Не удалось сохранить файл";
    }

    private void LoadGarage()
    {
        using var dlg = new OpenFileDialog { Filter = "Архив гаража (*.txt)|*.txt|Все файлы (*.*)|*.*" };
        if (dlg.ShowDialog() != DialogResult.OK) return;

        bool ok = garage.Load(dlg.FileName);
        RefreshList();
        lblStatus.Text = ok ? "Данные загружены" : "Не удалось загрузить файл";
    }

    private void RefreshList()
    {
        listCars.BeginUpdate();
        listCars.Items.Clear();
        for (int i = 0; i < garage.Count; i++)
        {
            var c = garage.GetCar(i);
            listCars.Items.Add($"{i + 1}. {c.Team} - {c.Driver} ({c.Year})" + (c.IsExtended ? " *" : ""));
        }
        listCars.EndUpdate();
        lblStatus.Text = $"Машин в гараже: {garage.Count}";
    }

    private void ShowSelected()
    {
        if (listCars.SelectedIndex < 0) return;
        var c = garage.GetCar(listCars.SelectedIndex);
        txtTeam.Text = c.Team;
        txtDriver.Text = c.Driver;
        numYear.Value = Math.Clamp(c.Year, (int)numYear.Minimum, (int)numYear.Maximum);
        numPower.Value = Math.Clamp(c.Power, (int)numPower.Minimum, (int)numPower.Maximum);
        numWeight.Value = Math.Clamp((decimal)c.Weight, numWeight.Minimum, numWeight.Maximum);
        chkExtended.Checked = c.IsExtended;
        txtPrincipal.Text = c.Principal;
        numBudget.Value = Math.Clamp((decimal)c.Budget, numBudget.Minimum, numBudget.Maximum);
        txtSponsors.Text = c.Sponsors.Replace("|", ", ");
    }
}
