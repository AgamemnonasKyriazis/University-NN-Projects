y = load("-ascii", "data.txt");
y = transpose(y);
scatter(y(1, :), y(2, :), "+")